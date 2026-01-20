<script lang="ts">
  import { onMount, onDestroy } from "svelte";
  import { browser } from "$app/environment";

  export let jsUrl;
  export let wasmUrl = "";

  let container;
  let canvas;
  let setDimensions;

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
    if (!canvas || !container) return;

    const w = container.clientWidth;
    const h = container.clientHeight;
    //canvas.style.width = `${w}px`;
    //canvas.style.height = `${h}px`;
    setDimensions(w, h);
  }

  onMount(async () => {
    if (!browser) return;
    if (!jsUrl) return;

    const basePath = dirname(jsUrl);

    globalThis.Module = {
      canvas,

      locateFile: (path) => {
        if (wasmUrl && path.endsWith(".wasm")) return wasmUrl;
        return basePath + path;
      },

      onRuntimeInitialized() {
        setDimensions = Module.cwrap(
          "set_dimensions",
          "void",
          ["number", "number"]
        )
        resizeToFit();
      }
    };

    await loadScript(
      "https://cdn.jsdelivr.net/gh/eligrey/FileSaver.js/dist/FileSaver.min.js"
    );

    window.addEventListener('resize', resizeToFit);

    await loadScript(jsUrl);
  });

  onDestroy(() => {
    if (browser) delete globalThis.Module;
  });
</script>

<div
  class="relative flex-1 min-h-0 rounded-xl overflow-hidden"
  bind:this={container}
>
  <canvas
    id={canvasId}
    bind:this={canvas}
    class="emscripten w-full h-full min-h-0 min-w-0 m-0"
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
