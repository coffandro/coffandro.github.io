<script lang="ts">
  import { onMount, onDestroy } from "svelte";
  import { browser } from "$app/environment";

  export let jsUrl;
  export let wasmUrl = "";
  export let assetsUrl = ""; // URL to assets.json manifest, defaults to basePath + "assets.json"
  export let onGameEvent: ((type: number, data: number) => void) | undefined = undefined;
  export function unlock() { unfocus(); }
  export function lock() { focus(); }

  let container: HTMLDivElement;
  let canvas: HTMLCanvasElement;
  let setDimensions: (w: number, h: number) => void;
  let cursorLock: () => void;
  let cursorUnlock: () => void;
  let clickedInCanvas = false;
  let gameScript: HTMLScriptElement | null = null;
  let crashed = false;

  const canvasId = `raylib-canvas-${Math.random().toString(36).slice(2)}`;

  function isScriptLoaded(src: string): boolean {
    if (!browser) return false;
    const resolved = new URL(src, location.href).href;
    return [...document.scripts].some(s => s.src === resolved);
  }

  function loadScript(src: string): Promise<HTMLScriptElement> {
    return new Promise((resolve, reject) => {
      if (!browser) return resolve(null as any);

      const s = document.createElement("script");
      s.src = src;
      s.async = true;
      s.onload = () => resolve(s);
      s.onerror = reject;
      document.body.appendChild(s);
    });
  }

  function removeScriptBySrc(src: string) {
    const resolved = new URL(src, location.href).href;
    for (const s of [...document.scripts]) {
      if (s.src === resolved) s.remove();
    }
  }

  function dirname(path) {
    return path.substring(0, path.lastIndexOf("/") + 1);
  }

  async function fetchAssetManifest(url) {
    try {
      const res = await fetch(url);
      if (!res.ok) return null;
      return await res.json();
    } catch {
      return null;
    }
  }

  async function loadAssetsIntoFS(basePath, manifest) {
    const FS = globalThis.Module.FS;
    if (!FS || !manifest?.files) return;

    // Create /tmp/assets directory structure
    try {
      FS.mkdir("/tmp/assets");
    } catch (e) {
      // Directory might already exist
    }

    // Helper to create nested directories
    function ensureDir(path) {
      const parts = path.split("/").filter(Boolean);
      let current = "";
      for (const part of parts) {
        current += "/" + part;
        try {
          FS.mkdir(current);
        } catch (e) {
          // Directory exists
        }
      }
    }

    // Load each asset file
    for (const filePath of manifest.files) {
      const url = basePath + "assets/" + filePath;
      try {
        const res = await fetch(url);
        if (!res.ok) {
          console.warn(`Failed to load asset: ${filePath}`);
          continue;
        }
        const data = await res.arrayBuffer();

        // Ensure parent directories exist
        const destPath = "/tmp/assets/" + filePath;
        const parentDir = destPath.substring(0, destPath.lastIndexOf("/"));
        if (parentDir && parentDir !== "/tmp/assets") {
          ensureDir(parentDir);
        }

        // Write the file
        FS.writeFile(destPath, new Uint8Array(data));
        console.log(`Loaded asset: ${destPath}`);
      } catch (e) {
        console.warn(`Error loading asset ${filePath}:`, e);
      }
    }
  }

  function resizeToFit() {
    if (!canvas || !container) return;

    const w = container.clientWidth;
    const h = container.clientHeight;
    setDimensions(w, h);
  }

  function focus() {
    clickedInCanvas = true;
    if (cursorLock) cursorLock();
  }

  function unfocus() {
    clickedInCanvas = false;
    if (cursorUnlock) cursorUnlock();
  }

  function handlePointerLockChange() {
    if (document.pointerLockElement !== canvas) {
      clickedInCanvas = false;
    }
  }

  function handleKeyDown(ev: KeyboardEvent) {
    if (clickedInCanvas) {
      if (
        ["Space", "ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight"].includes(
          ev.code,
        )
      ) {
        ev.preventDefault();
      }
    }
    if (ev.code === "Escape" && clickedInCanvas) {
      unfocus();
    }
  }

  function handleWindowClick(ev: MouseEvent) {
    if (clickedInCanvas && !container.contains(ev.target as Node)) {
      unfocus();
    }
  }

  function handleGameEvent(ev: Event) {
    const detail = (ev as CustomEvent).detail;
    if (onGameEvent) onGameEvent(detail.type, detail.data);
  }

  function handleOverlayClick() {
    focus();
  }

  let cleanupListeners: (() => void) | undefined;

  onMount(async () => {
    if (!browser) return;
    if (!jsUrl) return;

    const basePath = dirname(jsUrl);
    const manifestUrl = assetsUrl || basePath + "assets.json";

    // Fetch asset manifest before initializing module
    const manifest = await fetchAssetManifest(manifestUrl);

    globalThis.Module = {
      canvas,

      onAbort() {
        crashed = true;
      },

      locateFile: (path) => {
        if (wasmUrl && path.endsWith(".wasm")) return wasmUrl;
        return basePath + path;
      },

      // preRun uses addRunDependency to block main() until assets load
      preRun: [
        function () {
          if (manifest && manifest.files && manifest.files.length > 0) {
            Module.addRunDependency("assets");
            loadAssetsIntoFS(basePath, manifest)
              .then(() => {
                Module.removeRunDependency("assets");
              })
              .catch((e) => {
                console.error("Failed to load assets:", e);
                Module.removeRunDependency("assets");
              });
          }
        },
      ],

      onRuntimeInitialized() {
        setDimensions = Module.cwrap("set_dimensions", "void", [
          "number",
          "number",
        ]);
        cursorLock = Module.cwrap("cursor_lock", "void", []);
        cursorUnlock = Module.cwrap("cursor_unlock", "void", []);
        resizeToFit();
      },
    };

    const fileSaverUrl = "https://cdn.jsdelivr.net/gh/eligrey/FileSaver.js/dist/FileSaver.min.js";
    if (!isScriptLoaded(fileSaverUrl)) {
      await loadScript(fileSaverUrl);
    }

    window.addEventListener("resize", resizeToFit);
    // Capture phase so we get Escape before emscripten's handlers
    window.addEventListener("keydown", handleKeyDown, true);
    window.addEventListener("click", handleWindowClick);
    document.addEventListener("pointerlockchange", handlePointerLockChange);
    canvas.addEventListener("game-event", handleGameEvent);

    cleanupListeners = () => {
      window.removeEventListener("resize", resizeToFit);
      window.removeEventListener("keydown", handleKeyDown, true);
      window.removeEventListener("click", handleWindowClick);
      document.removeEventListener(
        "pointerlockchange",
        handlePointerLockChange,
      );
      canvas.removeEventListener("game-event", handleGameEvent);
    };

    // Remove stale game script from a previous mount before loading fresh
    removeScriptBySrc(jsUrl);
    gameScript = await loadScript(jsUrl);
  });

  onDestroy(() => {
    if (browser) {
      // Try to stop the emscripten main loop before tearing down
      const mod = (globalThis as Record<string, any>).Module;
      if (mod) {
        try { mod.pauseMainLoop?.(); } catch {}
        try { mod.abort?.(); } catch {}
      }
      delete (globalThis as Record<string, unknown>).Module;
      if (gameScript) {
        gameScript.remove();
        gameScript = null;
      }
      if (cleanupListeners) cleanupListeners();
    }
  });
</script>

<div
  class="relative flex-1 min-h-0 rounded-xl overflow-hidden"
  bind:this={container}
>
  <canvas
    id={canvasId}
    bind:this={canvas}
    class="emscripten w-full h-full min-h-0 min-w-0 m-0 cursor-default"
    tabindex="-1"
    on:contextmenu|preventDefault
  />
  {#if crashed}
    <div class="overlay crash-overlay">
      <p>The game crashed. Please reload the page.</p>
      <button on:click={() => location.reload()}>Reload</button>
    </div>
  {:else if !clickedInCanvas}
    <button class="overlay" on:click|stopPropagation={handleOverlayClick}>
      Click to play
    </button>
  {/if}
</div>

<style>
  canvas.emscripten {
    display: block;
    background: black;
    image-rendering: pixelated;
  }
  .overlay {
    position: absolute;
    inset: 0;
    display: flex;
    align-items: center;
    justify-content: center;
    background: rgba(0, 0, 0, 0.3);
    color: white;
    font-size: 1.25rem;
    cursor: pointer;
    border: none;
    width: 100%;
  }
  .crash-overlay {
    flex-direction: column;
    gap: 0.75rem;
    background: rgba(0, 0, 0, 0.7);
    cursor: default;
  }
  .crash-overlay button {
    padding: 0.5rem 1rem;
    border: 1px solid white;
    background: transparent;
    color: white;
    cursor: pointer;
    border-radius: 0.25rem;
  }
</style>
