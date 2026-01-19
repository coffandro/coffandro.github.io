<script>
  import { onMount, onDestroy } from "svelte";
  import { browser } from "$app/environment";

  export let jsUrl;
  export let wasmUrl = "";

  let container;
  let canvas;
  let resizeObserver;
  let aspectRatio = 1;

  const canvasId = `raylib-canvas-${Math.random().toString(36).slice(2)}`;

  function loadScript(src) {
    return new Promise((resolve, reject) => {
      if (!browser) return resolve();

      const s = document.createElement("script");
      s.src = src;
      s.async = true;
      s.onload = resolve;
      s.onerror = reject;
      document.body.appendChild(s);
    });
  }

  function dirname(path) {
    return path.substring(0, path.lastIndexOf("/") + 1);
  }

  function resizeToFit() {
    if (!canvas || !container || !aspectRatio) return;

    const w = container.clientWidth;
    canvas.style.width = `${w}px`;
    canvas.style.height = `${w / aspectRatio}px`;
  }

  onMount(async () => {
    if (!browser) return;
    if (!jsUrl) return;

    const basePath = dirname(jsUrl);

    // ⚠️ DO NOT touch window at top-level
    globalThis.Module = {
      canvas,

      locateFile: (path) => {
        if (wasmUrl && path.endsWith(".wasm")) return wasmUrl;
        return basePath + path;
      },

      onRuntimeInitialized() {
        aspectRatio = canvas.width / canvas.height || 1;
        resizeToFit();
      }
    };

    await loadScript(
      "https://cdn.jsdelivr.net/gh/eligrey/FileSaver.js/dist/FileSaver.min.js"
    );

    await loadScript(jsUrl);

    resizeObserver = new ResizeObserver(resizeToFit);
    resizeObserver.observe(container);
  });

  onDestroy(() => {
    resizeObserver?.disconnect();
    if (browser) delete globalThis.Module;
  });
</script>

<div
  class="grow rounded-xl overflow-hidden"
  bind:this={container}
>
  <canvas
    id={canvasId}
    bind:this={canvas}
    class="emscripten"
    tabindex="-1"
    on:contextmenu|preventDefault
  />
</div>

<style>
  canvas.emscripten {
    display: block;
    background: black;
    image-rendering: pixelated;
  }
</style>
