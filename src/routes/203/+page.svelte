<script lang="ts">
    import GameEmbed from "$lib/components/game-embed.svelte";
</script>

<svelte:head>
    <title>203 page</title>
</svelte:head>

<div class="grow flex flex-col gap-2 min-h-0 mb-2">
    <div class="border-3 p-2 rounded-xl">
        <h1 class="text-3xl">203 Redesign!</h1>
        <p>
            Below sits my mockup made in blender for how we on KMG should build
            up the 203 cozy/studio room.
        </p>
        <p>THIS IS MOSTLY CONCEPT! I'm very flexible on this</p>
    </div>

    <div class="border-3 p-0 grow flex rounded-xl min-h-0 overflow-hidden">
        <GameEmbed jsUrl="/203/game/index.js" />
    </div>
</div>
