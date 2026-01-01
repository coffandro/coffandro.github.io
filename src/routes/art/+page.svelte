<script lang="ts">
    import ArtPiece from "$lib/components/art/art-piece.svelte";

    type ArtPiece = {
        title: string;
        imageFile: string;
    }

    const pieces: ArtPiece[] = [
        {
            title: "Someone forgot their plushie in a cave :(",
            imageFile: "monkey_art.png"
        },
        {
            title: "Remember your god damned space suit!",
            imageFile: "space_suit.png"
        },
        {
            title: "Sitting at desk",
            imageFile: "sitting_at_desk.png"
        },
        {
            title: "A little voxel avatar of me",
            imageFile: "coffandro.png"
        },
        {
            title: '"Torture Hardware" a metal album cover',
            imageFile: "torture_hardware.png"
        }
    ];
</script>

<svelte:head>
    <title>Art</title>
</svelte:head>

<div class="w-full flex flex-col mb-5">
    <div class="mb-2">
        <h1 class="text-3xl">Art!</h1>
        <p>I sometimes end up doing artistic things, mostly 3D, below are a few of my "pieces"</p>
    </div>

    <div class="grid grid-cols-2 md:grid-cols-3 w-full gap-4">
        {#each pieces as piece}
            <ArtPiece
                title={piece.title}
                imagePath="/pieces/{piece.imageFile}"
            />
        {/each}
    </div>
</div>
