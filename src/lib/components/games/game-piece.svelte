<script lang="ts">
    import type { Game } from "$lib/game";
    import Modal from "../modal.svelte";

    export let game: Game, classes: string = "";

    let playing_game: boolean = false;

    function buttonClick() {
        playing_game = !playing_game;
    }
</script>

<button command="show-modal" commandfor="{game.title}-dialog" class="p-2 bg-mantle-bg hover:bg-crust-bg rounded-lg flex flex-col justify-between cursor-pointer {classes}">
    <p class="text-center mb-2">{game.title}</p>
    <div class="aspect-square flex justify-center items-center">
        {#if game.logo_path}
            <img src={game.logo_path} class="rounded-lg">
        {/if}
    </div>
</button>

{#snippet topbar()}
    <h3 id="{game.title}-dialog-title" class="text-base font-semibold text-xl text-white">{game.title}</h3>
    <button type="button" command="close" commandfor="{game.title}-dialog" class="flex flex-col content-center justify-center rounded-md bg-gray-500 text-sm font-semibold text-white hover:bg-gray-400 ml-3 p-3 w-auto cursor-pointer">Close</button>
{/snippet}

<Modal
    topbar={topbar}
    id={game.title}
>
    <div class="w-full sm:grid grid-cols-2 gap-4">
        <div class="overflow-hidden rounded-xl">
            {#if game.itch_path && playing_game}
                <iframe frameborder="0" src={game.itch_path} allowfullscreen="" width="640" height="480" style="width:100%; height:400px;overflow:hidden;" title={game.title}></iframe>
            {:else}
                {#if game.youtupe_path}
                    <iframe width="1920" height="1080" src="{game.youtupe_path};controls=0" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen style="width:100%; height:400px;overflow:hidden;"></iframe>
                {:else if game.image_path}
                    <img src={game.image_path}>
                {:else if game.logo_path}
                    <img src={game.logo_path}>
                {/if}
            {/if}
        </div>

        <div class="text-base flex flex-col justify-between">
            <p class="whitespace-pre-wrap mt-5 sm:mt-0">{game.desc}</p>

            <div class="flex gap-2 mt-2">
                {#if game.link_path}
                    <a class="bg-surface-bg p-2 rounded-lg grow text-center" href={game.link_path}>Visit Page</a>
                {/if}

                {#if game.source_path}
                    <a class="bg-surface-bg p-2 rounded-lg grow text-center" href={game.source_path}>See code</a>
                {/if}

                {#if game.itch_path}
                    {#if playing_game}
                        <button on:click={buttonClick} class="bg-surface-bg p-2 rounded-lg grow text-center cursor-pointer">Return to {game.youtupe_path ? "video" : "image"}</button>
                    {:else}
                        <button on:click={buttonClick} class="bg-surface-bg p-2 rounded-lg grow text-center cursor-pointer">Play Game</button>
                    {/if}
                {/if}
            </div>
        </div>
    </div>
</Modal>