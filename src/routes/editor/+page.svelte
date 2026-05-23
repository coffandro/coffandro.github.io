<script lang="ts">
    import MarkdownEditor from "$lib/components/markdown-editor.svelte";

    let value = $state("");
    let editor = $state<
        | {
              insertAtCursor: (text: string) => void;
              prependLine: (text: string) => void;
              appendLine: (text: string, newline?: boolean) => void;
              insertNewLine: (text?: string) => void;
              wrapSelection: (before: string, after?: string) => void;
              toggleTaskChecked: () => void;
              focus: () => void;
          }
        | undefined
    >();

    const btn =
        "border-2 rounded-lg px-2 py-1 hover:bg-hover hover:cursor-pointer text-sm";
</script>

<svelte:head>
    <title>Editor</title>
</svelte:head>

<div class="w-full flex flex-col mb-5">
    <div class="w-full border-3 border-text p-2 mb-2 rounded-xl">
        <div class="mb-2">
            <h1 class="text-3xl">Editor</h1>
            <p>Live markdown editor trululu.</p>
        </div>
    </div>

    <div class="flex flex-col gap-2 w-full border-3 border-text p-2 rounded-xl">
        <MarkdownEditor bind:value bind:api={editor} class="min-h-40" />

        <div class="flex flex-wrap gap-1 border-t-2 border-text pt-2">
            <button
                class={btn}
                title="Heading 1"
                onclick={() => editor?.appendLine("# ", true)}>H1</button
            >
            <button
                class={btn}
                title="Heading 2"
                onclick={() => editor?.appendLine("## ", true)}>H2</button
            >
            <button
                class={btn}
                title="Heading 3"
                onclick={() => editor?.appendLine("### ", true)}>H3</button
            >
            <button
                class={btn}
                title="Subtext"
                onclick={() => editor?.appendLine("-# ", true)}>-#</button
            >

            <span class="w-px bg-text/30 mx-1"></span>

            <button
                class={btn}
                title="Bold"
                onclick={() => editor?.wrapSelection("**")}><b>B</b></button
            >
            <button
                class={btn}
                title="Italic"
                onclick={() => editor?.wrapSelection("*")}><i>I</i></button
            >
            <button
                class={btn}
                title="Strikethrough"
                onclick={() => editor?.wrapSelection("~~")}><s>S</s></button
            >
            <button
                class={btn}
                title="Inline code"
                onclick={() => editor?.wrapSelection("`")}>{`</>`}</button
            >

            <span class="w-px bg-text/30 mx-1"></span>

            <button
                class={btn}
                title="Link"
                onclick={() => editor?.wrapSelection("[", "](url)")}
                >Link</button
            >
            <button
                class={btn}
                title="Image"
                onclick={() => editor?.wrapSelection("![", "](url)")}
                >Image</button
            >

            <span class="w-px bg-text/30 mx-1"></span>

            <button
                class={btn}
                title="Bullet list"
                onclick={() => editor?.appendLine("- ", true)}>• List</button
            >
            <button
                class={btn}
                title="Numbered list"
                onclick={() => editor?.appendLine("1. ", true)}>1. List</button
            >
            <button
                class={btn}
                title="Checkbox"
                onclick={() => editor?.appendLine("- [ ] ", true)}>☐</button
            >
            <button
                class={btn}
                title="Toggle checkbox"
                onclick={() => editor?.toggleTaskChecked()}>☑</button
            >

            <span class="w-px bg-text/30 mx-1"></span>

            <button
                class={btn}
                title="Blockquote"
                onclick={() => editor?.appendLine("> ", true)}>&gt;</button
            >
            <button
                class={btn}
                title="Horizontal rule"
                onclick={() => editor?.insertNewLine("---")}>—</button
            >
        </div>
    </div>
</div>
