<script lang="ts">
    import ArtPiece from "$lib/components/art/art-piece.svelte";
    import GameEmbed from "$lib/components/game-embed.svelte";
    import { onMount } from "svelte";

    type ArtPiece = {
        title: string;
        imageFile: string;
    };

    const pieces: ArtPiece[] = [
        {
            title: "Someone forgot their plushie in a cave :(",
            imageFile: "monkey_art.png",
        },
        {
            title: "Remember your god damned space suit!",
            imageFile: "space_suit.png",
        },
        {
            title: "Sitting at desk",
            imageFile: "sitting_at_desk.png",
        },
        {
            title: "A little voxel avatar of me",
            imageFile: "coffandro.png",
        },
        {
            title: '"Torture Hardware" a metal album cover',
            imageFile: "torture_hardware.png",
        },
    ];

    let showGame: boolean = $state(false);
    let mounted = false;

    onMount(() => {
        const saved = localStorage.getItem("artShowGame");
        showGame = saved !== null ? saved === "true" : true;
        mounted = true;
    });

    $effect(() => {
        if (mounted) {
            localStorage.setItem("artShowGame", String(showGame));
        }
    });
</script>

<svelte:head>
    <title>Art</title>
</svelte:head>

<div class="grow flex flex-col gap-2 min-h-0 mb-2">
    <div class="border-3 p-2 rounded-xl">
        <h1 class="text-3xl">Art!</h1>
        <p>
            I sometimes end up doing artistic things, mostly 3D, below are a few
            of my "pieces" in a gallery game
        </p>

        <div class="flex">
            <label>
                <input type="checkbox" bind:checked={showGame} />
                Use Gallery Game
            </label>
        </div>
    </div>

    <div
        class="border-3 p-0 grow flex rounded-xl min-h-0 overflow-hidden"
        class:hidden={!showGame}
    >
        <GameEmbed jsUrl="/art/game/index.js" />
    </div>
    {#if !showGame}
        <div
            class="grid grid-cols-2 md:grid-cols-3 w-full gap-4 border-3 border-text p-2 rounded-xl"
            class:hidden={showGame}
        >
            {#each pieces as piece}
                <ArtPiece
                    title={piece.title}
                    imagePath="/pieces/{piece.imageFile}"
                />
            {/each}
        </div>
    {/if}
</div>
