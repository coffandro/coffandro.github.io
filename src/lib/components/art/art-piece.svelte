<script lang="ts">
    import Modal from "../modal.svelte";

    export let title: string, imagePath: string, classes: string = "";
</script>

<button command="show-modal" commandfor="{title}-dialog" class="p-2 hover:bg-hover rounded-lg flex flex-col justify-between cursor-pointer border-2 border-text font-semibold {classes}">
    <p class="text-center mb-2">{title}</p>
    <div class="aspect-square flex justify-center items-center">
        <img src={imagePath} class="rounded-lg">
    </div>
</button>

{#snippet topbar()}
    <h3 id="dialog-title" class="font-semibold text-xl">{title}</h3>
    <button type="button" command="close" commandfor="{title}-dialog" class="flex flex-col content-center justify-center rounded-md text-sm font-semibold border border-text hover:bg-hover cursor-pointer ml-3 p-3 w-auto">Close</button>
{/snippet}

<Modal
    topbar={topbar}
    id={title}
>
    <div class="flex justify-center">
        <img src={imagePath} class="rounded-lg max-h-[80vh]">
    </div>
</Modal>