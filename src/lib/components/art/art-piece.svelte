<script lang="ts">
    import Modal from "../modal.svelte";

    export let title: string, imagePath: string, classes: string = "";
</script>

<button command="show-modal" commandfor="{title}-dialog" class="p-2 bg-mantle-bg hover:bg-crust-bg rounded-lg flex flex-col justify-between cursor-pointer {classes}">
    <p class="text-center mb-2">{title}</p>
    <div class="aspect-square flex justify-center items-center">
        <img src={imagePath} class="rounded-lg">
    </div>
</button>

{#snippet topbar()}
    <div class="w-full bg-gray-700/25 px-4 py-3 flex justify-between">
        <h3 id="dialog-title" class="text-base font-semibold text-xl text-white">{title}</h3>
        <button type="button" command="close" commandfor="{title}-dialog" class="flex flex-col content-center justify-center rounded-md bg-gray-500 text-sm font-semibold text-white hover:bg-gray-400 ml-3 p-3 w-auto">Close</button>
    </div>
{/snippet}

<Modal
    topbar={topbar}
    id={title}
>
    <div class="flex justify-center">
        <img src={imagePath} class="rounded-lg max-h-[80vh]">
    </div>
</Modal>