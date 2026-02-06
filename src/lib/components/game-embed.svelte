<script lang="ts">
  import { onMount, onDestroy } from "svelte";
  import { browser } from "$app/environment";

  export let jsUrl;
  export let wasmUrl = "";
  export let assetsUrl = ""; // URL to assets.json manifest, defaults to basePath + "assets.json"

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
    //canvas.style.width = `${w}px`;
    //canvas.style.height = `${h}px`;
    setDimensions(w, h);
  }

  onMount(async () => {
    if (!browser) return;
    if (!jsUrl) return;

    const basePath = dirname(jsUrl);
    const manifestUrl = assetsUrl || (basePath + "assets.json");

    // Fetch asset manifest before initializing module
    const manifest = await fetchAssetManifest(manifestUrl);

    globalThis.Module = {
      canvas,

      locateFile: (path) => {
        if (wasmUrl && path.endsWith(".wasm")) return wasmUrl;
        return basePath + path;
      },

      // preRun uses addRunDependency to block main() until assets load
      preRun: [
        function() {
          if (manifest && manifest.files && manifest.files.length > 0) {
            Module.addRunDependency('assets');
            loadAssetsIntoFS(basePath, manifest).then(() => {
              Module.removeRunDependency('assets');
            }).catch((e) => {
              console.error('Failed to load assets:', e);
              Module.removeRunDependency('assets');
            });
          }
        }
      ],

      onRuntimeInitialized() {
        setDimensions = Module.cwrap(
          "set_dimensions",
          "void",
          ["number", "number"]
        );
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
