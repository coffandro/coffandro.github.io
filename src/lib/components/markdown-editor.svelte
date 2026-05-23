<script lang="ts">
    import { onMount } from "svelte";

    type EditorApi = {
        insertAtCursor: (text: string) => void;
        prependLine: (text: string) => void;
        appendLine: (text: string, newline?: boolean) => void;
        insertNewLine: (text?: string) => void;
        wrapSelection: (before: string, after?: string) => void;
        toggleTaskChecked: () => void;
        focus: () => void;
    };

    let {
        value = $bindable(""),
        api = $bindable<EditorApi | undefined>(),
        class: className = "",
        ...rest
    } = $props();

    let editorRef: HTMLDivElement;

    type Caret = { lineIndex: number; lineOffset: number } | null;

    type Segment =
        | { type: "text"; content: string }
        | { type: "bold"; marker: "**" | "__"; content: string }
        | { type: "italic"; marker: "*" | "_"; content: string }
        | { type: "boldItalic"; marker: "***" | "___"; content: string }
        | { type: "strike"; content: string }
        | { type: "code"; content: string }
        | { type: "link"; text: string; url: string }
        | { type: "image"; alt: string; url: string };

    type BlockKind =
        | { kind: "p" }
        | { kind: "h"; level: number; marker: string }
        | { kind: "sub"; marker: string }
        | { kind: "bq"; marker: string }
        | { kind: "ul"; marker: string }
        | { kind: "ol"; marker: string }
        | { kind: "task"; marker: string; checked: boolean }
        | { kind: "hr"; marker: string };

    const HEADING_PARSE = /^(#{1,6})(?: (.*))?$/;
    const SUBTEXT_PARSE = /^-#(?: (.*))?$/;
    const BLOCKQUOTE_PARSE = /^>(?: (.*))?$/;
    const TASK_PARSE = /^- \[([ xX])\] (.*)$/;
    const UL_PARSE = /^([-*+]) (.*)$/;
    const OL_PARSE = /^(\d+)\. (.*)$/;
    const HR_PARSE = /^(-{3,}|\*{3,}|_{3,})$/;

    function detectBlock(text: string): { block: BlockKind; content: string } {
        let m: RegExpMatchArray | null;
        if ((m = text.match(HR_PARSE))) {
            return { block: { kind: "hr", marker: m[1] }, content: "" };
        }
        if ((m = text.match(HEADING_PARSE))) {
            const marker = m[1] + (m[2] !== undefined ? " " : "");
            return {
                block: { kind: "h", level: m[1].length, marker },
                content: m[2] ?? "",
            };
        }
        if ((m = text.match(SUBTEXT_PARSE))) {
            const marker = "-#" + (m[1] !== undefined ? " " : "");
            return { block: { kind: "sub", marker }, content: m[1] ?? "" };
        }
        if ((m = text.match(BLOCKQUOTE_PARSE))) {
            const marker = m[1] !== undefined ? "> " : ">";
            return { block: { kind: "bq", marker }, content: m[1] ?? "" };
        }
        if ((m = text.match(TASK_PARSE))) {
            const checked = m[1] !== " ";
            return {
                block: {
                    kind: "task",
                    marker: `- [${m[1]}] `,
                    checked,
                },
                content: m[2],
            };
        }
        if ((m = text.match(UL_PARSE))) {
            return {
                block: { kind: "ul", marker: m[1] + " " },
                content: m[2],
            };
        }
        if ((m = text.match(OL_PARSE))) {
            return {
                block: { kind: "ol", marker: m[1] + ". " },
                content: m[2],
            };
        }
        return { block: { kind: "p" }, content: text };
    }

    function parseInline(text: string): Segment[] {
        const segments: Segment[] = [];
        let plainStart = 0;
        let i = 0;

        const flush = (upTo: number) => {
            if (upTo > plainStart) {
                segments.push({
                    type: "text",
                    content: text.slice(plainStart, upTo),
                });
            }
        };

        while (i < text.length) {
            // ![alt](url)
            if (text[i] === "!" && text[i + 1] === "[") {
                const m = text
                    .slice(i)
                    .match(/^!\[([^\]\n]*)\]\(([^)\n]*)\)/);
                if (m) {
                    flush(i);
                    segments.push({
                        type: "image",
                        alt: m[1],
                        url: m[2],
                    });
                    i += m[0].length;
                    plainStart = i;
                    continue;
                }
            }
            // [text](url)
            if (text[i] === "[") {
                const m = text
                    .slice(i)
                    .match(/^\[([^\]\n]*)\]\(([^)\n]*)\)/);
                if (m) {
                    flush(i);
                    segments.push({
                        type: "link",
                        text: m[1],
                        url: m[2],
                    });
                    i += m[0].length;
                    plainStart = i;
                    continue;
                }
            }
            // `code`
            if (text[i] === "`") {
                const end = text.indexOf("`", i + 1);
                if (end !== -1) {
                    flush(i);
                    segments.push({
                        type: "code",
                        content: text.slice(i + 1, end),
                    });
                    i = end + 1;
                    plainStart = i;
                    continue;
                }
            }
            // ***bold italic***
            if (
                (text[i] === "*" &&
                    text[i + 1] === "*" &&
                    text[i + 2] === "*") ||
                (text[i] === "_" &&
                    text[i + 1] === "_" &&
                    text[i + 2] === "_")
            ) {
                const marker = text.slice(i, i + 3) as "***" | "___";
                const end = text.indexOf(marker, i + 3);
                if (end !== -1) {
                    flush(i);
                    segments.push({
                        type: "boldItalic",
                        marker,
                        content: text.slice(i + 3, end),
                    });
                    i = end + 3;
                    plainStart = i;
                    continue;
                }
            }
            // **bold**
            if (
                (text[i] === "*" && text[i + 1] === "*") ||
                (text[i] === "_" && text[i + 1] === "_")
            ) {
                const marker = text.slice(i, i + 2) as "**" | "__";
                const end = text.indexOf(marker, i + 2);
                if (end !== -1) {
                    flush(i);
                    segments.push({
                        type: "bold",
                        marker,
                        content: text.slice(i + 2, end),
                    });
                    i = end + 2;
                    plainStart = i;
                    continue;
                }
            }
            // ~~strike~~
            if (text[i] === "~" && text[i + 1] === "~") {
                const end = text.indexOf("~~", i + 2);
                if (end !== -1) {
                    flush(i);
                    segments.push({
                        type: "strike",
                        content: text.slice(i + 2, end),
                    });
                    i = end + 2;
                    plainStart = i;
                    continue;
                }
            }
            // *italic* or _italic_
            if (
                (text[i] === "*" || text[i] === "_") &&
                text[i + 1] !== text[i]
            ) {
                const marker = text[i] as "*" | "_";
                let end = -1;
                for (let j = i + 2; j < text.length; j++) {
                    if (
                        text[j] === marker &&
                        text[j + 1] !== marker &&
                        text[j - 1] !== marker
                    ) {
                        end = j;
                        break;
                    }
                }
                if (end !== -1) {
                    flush(i);
                    segments.push({
                        type: "italic",
                        marker,
                        content: text.slice(i + 1, end),
                    });
                    i = end + 1;
                    plainStart = i;
                    continue;
                }
            }
            i++;
        }
        flush(text.length);
        return segments;
    }

    function buildSyntaxSpan(text: string): HTMLSpanElement {
        const span = document.createElement("span");
        span.className = "md-syntax";
        span.textContent = text;
        return span;
    }

    function buildPairWrap(
        className: string,
        marker: string,
        content: string,
    ): HTMLSpanElement {
        const wrap = document.createElement("span");
        wrap.className = className;
        wrap.appendChild(buildSyntaxSpan(marker));
        if (content) wrap.appendChild(document.createTextNode(content));
        wrap.appendChild(buildSyntaxSpan(marker));
        return wrap;
    }

    function appendInline(parent: HTMLElement, segments: Segment[]) {
        for (const seg of segments) {
            if (seg.type === "text") {
                parent.appendChild(document.createTextNode(seg.content));
            } else if (seg.type === "bold") {
                parent.appendChild(
                    buildPairWrap("md-bold", seg.marker, seg.content),
                );
            } else if (seg.type === "italic") {
                parent.appendChild(
                    buildPairWrap("md-italic", seg.marker, seg.content),
                );
            } else if (seg.type === "boldItalic") {
                parent.appendChild(
                    buildPairWrap("md-bolditalic", seg.marker, seg.content),
                );
            } else if (seg.type === "strike") {
                parent.appendChild(
                    buildPairWrap("md-strike", "~~", seg.content),
                );
            } else if (seg.type === "code") {
                parent.appendChild(
                    buildPairWrap("md-code", "`", seg.content),
                );
            } else if (seg.type === "link") {
                const wrap = document.createElement("span");
                wrap.className = "md-link";
                wrap.appendChild(buildSyntaxSpan("["));
                const txt = document.createElement("span");
                txt.className = "md-link-text";
                txt.textContent = seg.text;
                wrap.appendChild(txt);
                wrap.appendChild(buildSyntaxSpan("]("));
                const url = document.createElement("span");
                url.className = "md-syntax md-link-url";
                url.textContent = seg.url;
                wrap.appendChild(url);
                wrap.appendChild(buildSyntaxSpan(")"));
                parent.appendChild(wrap);
            } else if (seg.type === "image") {
                const wrap = document.createElement("span");
                wrap.className = "md-image";
                wrap.appendChild(buildSyntaxSpan("!["));
                const alt = document.createElement("span");
                alt.className = "md-image-alt";
                alt.textContent = seg.alt;
                wrap.appendChild(alt);
                wrap.appendChild(buildSyntaxSpan("]("));
                const url = document.createElement("span");
                url.className = "md-syntax md-image-url";
                url.textContent = seg.url;
                wrap.appendChild(url);
                wrap.appendChild(buildSyntaxSpan(")"));
                parent.appendChild(wrap);
            }
        }
    }

    function buildLine(text: string): HTMLDivElement {
        const div = document.createElement("div");
        div.className = "md-line";

        const { block, content } = detectBlock(text);

        if (block.kind === "p") {
            const segments = parseInline(content);
            if (segments.length > 0) {
                appendInline(div, segments);
            } else {
                div.appendChild(document.createElement("br"));
            }
            return div;
        }

        if (block.kind === "h") div.classList.add(`md-h${block.level}`);
        else if (block.kind === "sub") div.classList.add("md-subtext");
        else if (block.kind === "bq") div.classList.add("md-blockquote");
        else if (block.kind === "ul") div.classList.add("md-list-ul");
        else if (block.kind === "ol") div.classList.add("md-list-ol");
        else if (block.kind === "task") {
            div.classList.add("md-task");
            if (block.checked) div.classList.add("md-task-checked");
        } else if (block.kind === "hr") div.classList.add("md-hr");

        const marker = document.createElement("span");
        marker.className =
            block.kind === "ol"
                ? "md-block-marker"
                : "md-syntax md-block-syntax";
        marker.textContent = block.marker;
        div.appendChild(marker);

        if (block.kind === "hr") return div;

        const segments = parseInline(content);
        if (segments.length > 0) {
            appendInline(div, segments);
        } else {
            div.appendChild(document.createElement("br"));
        }
        return div;
    }

    function getBlockKindFromDom(el: Element): BlockKind {
        const markerEl = el.querySelector(
            ":scope > .md-block-syntax, :scope > .md-block-marker",
        );
        const marker = markerEl?.textContent ?? "";

        for (let i = 1; i <= 6; i++) {
            if (el.classList.contains(`md-h${i}`)) {
                return { kind: "h", level: i, marker };
            }
        }
        if (el.classList.contains("md-subtext"))
            return { kind: "sub", marker };
        if (el.classList.contains("md-blockquote"))
            return { kind: "bq", marker };
        if (el.classList.contains("md-list-ul"))
            return { kind: "ul", marker };
        if (el.classList.contains("md-list-ol"))
            return { kind: "ol", marker };
        if (el.classList.contains("md-task"))
            return {
                kind: "task",
                marker,
                checked: el.classList.contains("md-task-checked"),
            };
        if (el.classList.contains("md-hr")) return { kind: "hr", marker };
        return { kind: "p" };
    }

    function segmentKey(seg: Segment): string {
        switch (seg.type) {
            case "text":
                return `T:${seg.content}`;
            case "bold":
                return `B${seg.marker}:${seg.content}`;
            case "italic":
                return `I${seg.marker}:${seg.content}`;
            case "boldItalic":
                return `BI${seg.marker}:${seg.content}`;
            case "strike":
                return `S:${seg.content}`;
            case "code":
                return `C:${seg.content}`;
            case "link":
                return `L:${seg.text}|${seg.url}`;
            case "image":
                return `IMG:${seg.alt}|${seg.url}`;
        }
    }

    function blockKey(block: BlockKind): string {
        switch (block.kind) {
            case "p":
                return "";
            case "h":
                return `H${block.level}:${block.marker}`;
            case "sub":
                return `SUB:${block.marker}`;
            case "bq":
                return `BQ:${block.marker}`;
            case "ul":
                return `UL:${block.marker}`;
            case "ol":
                return `OL:${block.marker}`;
            case "task":
                return `TASK:${block.marker}`;
            case "hr":
                return `HR:${block.marker}`;
        }
    }

    function structuralKey(text: string): string {
        const { block, content } = detectBlock(text);
        const parts: string[] = [];
        const bk = blockKey(block);
        if (bk) parts.push(bk);
        if (block.kind !== "hr") {
            for (const seg of parseInline(content)) parts.push(segmentKey(seg));
        }
        return JSON.stringify(parts);
    }

    function readPairSegmentText(el: Element): string {
        let out = "";
        for (const child of el.childNodes) {
            if (child.nodeType === Node.TEXT_NODE) {
                out += child.textContent ?? "";
            }
        }
        return out;
    }

    function readSegmentFromDom(el: HTMLElement): Segment | null {
        const firstSyntax = el.querySelector(":scope > .md-syntax");
        const open = firstSyntax?.textContent ?? "";

        if (el.classList.contains("md-bolditalic")) {
            const marker = open === "___" ? "___" : "***";
            return {
                type: "boldItalic",
                marker,
                content: readPairSegmentText(el),
            };
        }
        if (el.classList.contains("md-bold")) {
            const marker = open === "__" ? "__" : "**";
            return {
                type: "bold",
                marker,
                content: readPairSegmentText(el),
            };
        }
        if (el.classList.contains("md-italic")) {
            const marker = open === "_" ? "_" : "*";
            return {
                type: "italic",
                marker,
                content: readPairSegmentText(el),
            };
        }
        if (el.classList.contains("md-strike")) {
            return { type: "strike", content: readPairSegmentText(el) };
        }
        if (el.classList.contains("md-code")) {
            return { type: "code", content: readPairSegmentText(el) };
        }
        if (el.classList.contains("md-link")) {
            const t =
                el.querySelector(":scope > .md-link-text")?.textContent ?? "";
            const u =
                el.querySelector(":scope > .md-link-url")?.textContent ?? "";
            return { type: "link", text: t, url: u };
        }
        if (el.classList.contains("md-image")) {
            const a =
                el.querySelector(":scope > .md-image-alt")?.textContent ?? "";
            const u =
                el.querySelector(":scope > .md-image-url")?.textContent ?? "";
            return { type: "image", alt: a, url: u };
        }
        return null;
    }

    function domStructuralKey(el: Element): string | null {
        const parts: string[] = [];
        const block = getBlockKindFromDom(el);
        const blockMarkerEl = el.querySelector(
            ":scope > .md-block-syntax, :scope > .md-block-marker",
        );

        if (block.kind !== "p") {
            if (!blockMarkerEl) return null;
            parts.push(blockKey(block));
        } else if (blockMarkerEl) {
            return null;
        }

        if (block.kind === "hr") {
            for (const node of el.childNodes) {
                if (node === blockMarkerEl) continue;
                if (node.nodeName === "BR") continue;
                return null;
            }
            return JSON.stringify(parts);
        }

        for (const node of el.childNodes) {
            if (node === blockMarkerEl) continue;
            if (node.nodeName === "BR") continue;
            if (node.nodeType === Node.TEXT_NODE) {
                parts.push(`T:${node.textContent ?? ""}`);
            } else if (node instanceof HTMLElement) {
                const seg = readSegmentFromDom(node);
                if (!seg) return null;
                parts.push(segmentKey(seg));
            } else {
                return null;
            }
        }

        return JSON.stringify(parts);
    }

    function isInSync(el: Element, text: string): boolean {
        const actual = domStructuralKey(el);
        if (actual === null) return false;
        return actual === structuralKey(text);
    }

    function getCaret(): Caret {
        const sel = window.getSelection();
        if (!sel || sel.rangeCount === 0 || !editorRef) return null;
        const range = sel.getRangeAt(0);
        if (!editorRef.contains(range.startContainer)) return null;

        for (let i = 0; i < editorRef.children.length; i++) {
            const line = editorRef.children[i];
            if (
                line === range.startContainer ||
                line.contains(range.startContainer)
            ) {
                const pre = document.createRange();
                pre.selectNodeContents(line);
                pre.setEnd(range.startContainer, range.startOffset);
                return { lineIndex: i, lineOffset: pre.toString().length };
            }
        }
        return null;
    }

    function setCaret(caret: Caret) {
        if (!caret || !editorRef) return;
        const line = editorRef.children[caret.lineIndex];
        if (!line) return;

        let remaining = caret.lineOffset;
        let target: Node = line;
        let targetOffset = 0;
        let placed = false;

        const walker = document.createTreeWalker(line, NodeFilter.SHOW_TEXT);
        let node = walker.nextNode();
        while (node) {
            const len = node.textContent?.length ?? 0;
            if (remaining <= len) {
                target = node;
                targetOffset = remaining;
                placed = true;
                break;
            }
            remaining -= len;
            node = walker.nextNode();
        }

        if (!placed) {
            target = line;
            targetOffset = line.childNodes.length;
        }

        const range = document.createRange();
        range.setStart(target, targetOffset);
        range.collapse(true);
        const sel = window.getSelection();
        if (sel) {
            sel.removeAllRanges();
            sel.addRange(range);
        }
    }

    function readText(): string {
        if (!editorRef) return "";
        return Array.from(editorRef.children)
            .map((el) => el.textContent ?? "")
            .join("\n");
    }

    function updateActive() {
        if (!editorRef) return;
        const sel = window.getSelection();
        const range = sel && sel.rangeCount > 0 ? sel.getRangeAt(0) : null;
        const inside = range && editorRef.contains(range.startContainer);

        for (const child of editorRef.children) {
            const active =
                inside &&
                (child === range!.startContainer ||
                    child.contains(range!.startContainer));
            child.classList.toggle("md-active", !!active);
        }
    }

    function syncDom() {
        if (!editorRef) return;
        const caret = getCaret();
        const children = Array.from(editorRef.children);
        let caretLineRebuilt = false;

        for (let i = 0; i < children.length; i++) {
            const el = children[i] as HTMLElement;
            if (!el.classList.contains("md-line")) el.classList.add("md-line");

            const text = el.textContent ?? "";
            if (!isInSync(el, text)) {
                const newEl = buildLine(text);
                el.replaceWith(newEl);
                if (caret && caret.lineIndex === i) caretLineRebuilt = true;
            }
        }

        if (caretLineRebuilt) setCaret(caret);
    }

    function handleInput() {
        syncDom();
        value = readText();
        updateActive();
    }

    function handleSelection() {
        if (!editorRef) return;
        const sel = window.getSelection();
        if (!sel || sel.rangeCount === 0) return;
        if (!editorRef.contains(sel.getRangeAt(0).startContainer)) return;
        updateActive();
    }

    function fullRender(text: string) {
        if (!editorRef) return;
        editorRef.replaceChildren(...text.split("\n").map(buildLine));
        updateActive();
    }

    function rebuildLineAt(index: number, newText: string, caretOffset: number) {
        if (!editorRef) return;
        const line = editorRef.children[index];
        if (!line) return;
        const newEl = buildLine(newText);
        line.replaceWith(newEl);
        setCaret({ lineIndex: index, lineOffset: caretOffset });
        updateActive();
        value = readText();
    }

    function insertAtCursor(text: string) {
        if (!editorRef) return;
        editorRef.focus();
        const caret = getCaret();
        if (!caret) {
            // Append to end of last line
            const lastIndex = Math.max(0, editorRef.children.length - 1);
            const last = editorRef.children[lastIndex];
            const currentText = last?.textContent ?? "";
            rebuildLineAt(lastIndex, currentText + text, (currentText + text).length);
            return;
        }
        const line = editorRef.children[caret.lineIndex];
        const currentText = line?.textContent ?? "";
        const newText =
            currentText.slice(0, caret.lineOffset) +
            text +
            currentText.slice(caret.lineOffset);
        rebuildLineAt(
            caret.lineIndex,
            newText,
            caret.lineOffset + text.length,
        );
    }

    function prependLine(text: string) {
        if (!editorRef) return;
        editorRef.focus();
        const caret = getCaret();
        const index = caret?.lineIndex ?? 0;
        const line = editorRef.children[index];
        if (!line) return;
        const currentText = line.textContent ?? "";
        const newText = text + currentText;
        const newOffset = (caret?.lineOffset ?? 0) + text.length;
        rebuildLineAt(index, newText, newOffset);
    }

    function appendLine(text: string, newline: boolean = false) {
        if (!editorRef) return;
        editorRef.focus();
        const caret = getCaret();
        const index = caret?.lineIndex ?? editorRef.children.length - 1;
        const line = editorRef.children[index];
        if (!line) return;
        const currentText = line.textContent ?? "";

        if (newline && currentText.length > 0) {
            insertNewLine(text);
            return;
        }

        const newText = currentText + text;
        rebuildLineAt(index, newText, newText.length);
    }

    function insertNewLine(text: string = "") {
        if (!editorRef) return;
        editorRef.focus();
        const caret = getCaret();
        const newEl = buildLine(text);
        if (caret) {
            const line = editorRef.children[caret.lineIndex];
            line.after(newEl);
            setCaret({ lineIndex: caret.lineIndex + 1, lineOffset: text.length });
        } else {
            editorRef.appendChild(newEl);
            setCaret({
                lineIndex: editorRef.children.length - 1,
                lineOffset: text.length,
            });
        }
        updateActive();
        value = readText();
    }

    function wrapSelection(before: string, after: string = before) {
        if (!editorRef) return;
        editorRef.focus();
        const sel = window.getSelection();
        if (!sel || sel.rangeCount === 0) {
            insertAtCursor(before + after);
            return;
        }
        const range = sel.getRangeAt(0);
        if (!editorRef.contains(range.startContainer)) return;
        if (
            range.startContainer !== range.endContainer ||
            range.startOffset === range.endOffset
        ) {
            // No selection or cross-node — fall back to inserting markers
            insertAtCursor(before + after);
            return;
        }
        const selected = range.toString();
        // Find the line for this range
        let lineIndex = -1;
        let lineEl: Element | null = null;
        for (let i = 0; i < editorRef.children.length; i++) {
            const line = editorRef.children[i];
            if (line.contains(range.startContainer)) {
                lineIndex = i;
                lineEl = line;
                break;
            }
        }
        if (!lineEl || lineIndex === -1) return;
        const currentText = lineEl.textContent ?? "";
        const preRange = document.createRange();
        preRange.selectNodeContents(lineEl);
        preRange.setEnd(range.startContainer, range.startOffset);
        const startOffset = preRange.toString().length;
        const endOffset = startOffset + selected.length;
        const newText =
            currentText.slice(0, startOffset) +
            before +
            selected +
            after +
            currentText.slice(endOffset);
        rebuildLineAt(
            lineIndex,
            newText,
            startOffset + before.length + selected.length,
        );
    }

    function toggleTaskAt(lineIndex: number) {
        if (!editorRef) return;
        const line = editorRef.children[lineIndex] as HTMLElement | undefined;
        if (!line || !line.classList.contains("md-task")) return;

        const isChecked = line.classList.contains("md-task-checked");
        line.classList.toggle("md-task-checked", !isChecked);

        const markerEl = line.querySelector(
            ":scope > .md-block-syntax",
        ) as HTMLElement | null;
        if (markerEl) {
            markerEl.textContent = `- [${isChecked ? " " : "x"}] `;
        }

        value = readText();
    }

    function toggleTaskChecked() {
        if (!editorRef) return;
        const caret = getCaret();
        const index = caret?.lineIndex ?? editorRef.children.length - 1;
        toggleTaskAt(index);
    }

    function handleMouseDown(e: MouseEvent) {
        if (!editorRef) return;
        let target = e.target as Element | null;
        while (target && target.parentElement !== editorRef) {
            target = target.parentElement;
        }
        if (!target || !target.classList.contains("md-task")) return;
        if (target.classList.contains("md-active")) return;

        const rect = target.getBoundingClientRect();
        const xOffset = e.clientX - rect.left;
        const fontSize = parseFloat(getComputedStyle(target).fontSize);
        if (xOffset > fontSize * 1.8) return;

        const lineIndex = Array.from(editorRef.children).indexOf(target);
        if (lineIndex === -1) return;
        e.preventDefault();
        toggleTaskAt(lineIndex);
    }

    onMount(() => {
        fullRender(value ?? "");
        api = {
            insertAtCursor,
            prependLine,
            appendLine,
            insertNewLine,
            wrapSelection,
            toggleTaskChecked,
            focus: () => editorRef?.focus(),
        };
        document.addEventListener("selectionchange", handleSelection);
        return () =>
            document.removeEventListener("selectionchange", handleSelection);
    });
</script>

<div
    bind:this={editorRef}
    contenteditable="true"
    oninput={handleInput}
    onmousedown={handleMouseDown}
    class="md-editor {className}"
    {...rest}
></div>

<style>
    .md-editor {
        outline: none;
        white-space: pre-wrap;
        word-break: break-word;
        min-height: 1.5em;
    }

    .md-editor :global(.md-line) {
        min-height: 1em;
        line-height: 1.4;
    }

    .md-editor :global(.md-h1) {
        font-size: 2.25em;
        font-weight: 700;
        line-height: 1.2;
    }
    .md-editor :global(.md-h2) {
        font-size: 1.75em;
        font-weight: 700;
        line-height: 1.2;
    }
    .md-editor :global(.md-h3) {
        font-size: 1.5em;
        font-weight: 700;
    }
    .md-editor :global(.md-h4) {
        font-size: 1.25em;
        font-weight: 700;
    }
    .md-editor :global(.md-h5) {
        font-size: 1.125em;
        font-weight: 700;
    }
    .md-editor :global(.md-h6) {
        font-size: 1em;
        font-weight: 700;
    }

    .md-editor :global(.md-subtext) {
        font-size: 0.8em;
        opacity: 0.65;
    }

    .md-editor :global(.md-blockquote) {
        border-left: 3px solid currentColor;
        padding-left: 0.75em;
        opacity: 0.85;
        font-style: italic;
    }

    .md-editor :global(.md-list-ul),
    .md-editor :global(.md-list-ol) {
        padding-left: 1.5em;
        position: relative;
    }

    .md-editor :global(.md-list-ul:not(.md-active))::before {
        content: "•";
        position: absolute;
        left: 0.5em;
        opacity: 0.7;
    }

    .md-editor :global(.md-task) {
        padding-left: 1.5em;
        position: relative;
    }
    .md-editor :global(.md-task:not(.md-active))::before {
        content: "";
        position: absolute;
        left: 0.15em;
        top: 0.25em;
        width: 0.95em;
        height: 0.95em;
        border: 1.5px solid currentColor;
        border-radius: 3px;
        opacity: 0.7;
        box-sizing: border-box;
        cursor: pointer;
    }
    .md-editor :global(.md-task.md-task-checked:not(.md-active))::before {
        background: currentColor;
    }
    .md-editor :global(.md-task.md-task-checked:not(.md-active))::after {
        content: "✓";
        position: absolute;
        left: 0.25em;
        top: 0;
        color: var(--md-task-check-color, #fff);
        font-size: 0.85em;
        font-weight: 700;
        pointer-events: none;
    }
    .md-editor :global(.md-task.md-task-checked) {
        opacity: 0.6;
        text-decoration: line-through;
    }

    .md-editor :global(.md-hr) {
        position: relative;
        min-height: 1em;
    }
    .md-editor :global(.md-hr:not(.md-active))::after {
        content: "";
        position: absolute;
        top: 50%;
        left: 0;
        right: 0;
        border-top: 1px solid currentColor;
        opacity: 0.4;
    }

    .md-editor :global(.md-bold) {
        font-weight: 700;
    }
    .md-editor :global(.md-italic) {
        font-style: italic;
    }
    .md-editor :global(.md-bolditalic) {
        font-weight: 700;
        font-style: italic;
    }
    .md-editor :global(.md-strike) {
        text-decoration: line-through;
    }
    .md-editor :global(.md-code) {
        font-family: ui-monospace, SFMono-Regular, Menlo, monospace;
        background: rgba(128, 128, 128, 0.18);
        padding: 0.05em 0.3em;
        border-radius: 3px;
        font-size: 0.95em;
    }
    .md-editor :global(.md-link .md-link-text) {
        color: #2563eb;
        text-decoration: underline;
    }
    .md-editor :global(.md-image .md-image-alt) {
        font-style: italic;
        opacity: 0.75;
    }

    .md-editor :global(.md-syntax) {
        opacity: 0.45;
        font-weight: 400;
        font-style: normal;
    }
    .md-editor :global(.md-block-marker) {
        opacity: 0.7;
    }

    .md-editor :global(.md-line:not(.md-active) .md-syntax) {
        display: none;
    }
</style>
