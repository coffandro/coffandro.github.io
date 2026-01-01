<script lang="ts">
    import type { Site } from "$lib/site";
    import Modal from "../modal.svelte";

    export let site: Site, classes: string = "";
</script>

<button command="show-modal" commandfor="{site.title}-dialog" class="p-2 bg-bg hover:bg-bg rounded-lg flex flex-col justify-between cursor-pointer border-2 border-text {classes}">
    <p class="text-center mb-2">{site.title}</p>
    <div class="aspect-square flex justify-center items-center">
        {#if site.logo_path}
            <img src={site.logo_path} class="rounded-lg">
        {/if}
    </div>
</button>

{#snippet topbar()}
    <h3 id="{site.title}-dialog-title" class="text-base font-semibold text-xl text-white">{site.title}</h3>
    <button type="button" command="close" commandfor="{site.title}-dialog" class="flex flex-col content-center justify-center rounded-md bg-gray-500 text-sm font-semibold text-white hover:bg-gray-400 ml-3 p-3 w-auto cursor-pointer">Close</button>
{/snippet}

<Modal
    topbar={topbar}
    id={site.title}
>
    <div class="w-full gap-4">
        <div class="overflow-hidden rounded-xl">
            {#if site.image_path}
                <img src={site.image_path}>
            {:else if site.link_path}
                <iframe width="1920" height="1080" src="{site.link_path}" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen style="width:100%; height:400px;overflow:hidden;"></iframe>
            {/if}
        </div>

        <div class="text-base flex flex-col justify-between">
            <p class="whitespace-pre-wrap mt-5">{site.desc}</p>

            <div class="flex gap-2 mt-2">
                {#if site.link_path}
                    <a class="border border-text p-2 rounded-lg grow text-center" href={site.link_path}>Visit Page</a>
                {/if}

                {#if site.source_path}
                    <a class="border border-text p-2 rounded-lg grow text-center" href={site.source_path}>See code</a>
                {/if}
            </div>
        </div>
    </div>
</Modal>