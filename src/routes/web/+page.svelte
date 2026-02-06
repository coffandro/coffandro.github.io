<script lang="ts">
    import SitePiece from "$lib/components/sites/site-piece.svelte";
    import type { Site } from "$lib/site";

    const sites: Site[] = [
        {
            title: "SimplyPrint",
            desc: "As a part of my job as a Junior Software developer for SimplyPrint, I've worked on features such as the initial student dashboard feature and a printer integration.",
            link_path: "https://simplyprint.io/",
            logo_path: "/sites/simply_print.svg",
            image_path: "/sites/simply_print_screenshot.png",
        },
        {
            title: "BreadBird Games",
            desc: "As a founder of BreadBird Games and technical director I created the website to show off our previous experience and to facilitate new opportunities.",
            link_path: "https://breadbirdgames.github.io/",
            source_path: "https://github.com/BreadBirdGames/BreadBirdGames.github.io",
            logo_path: "/sites/bread_bird_games.png"
        },
        {
            title: "KATastrofe",
            desc: "In collaboration with Save the child Denmark I helped develop a board game to promote child safety on the internet, and the accompanying website.",
            link_path: "https://katastrofespil.github.io/",
            source_path: "https://github.com/KatastrofeSpil/KatastrofeSpil.github.io",
            logo_path: "/sites/katastrofe.svg"
        },
        {
            title: "Open Filament Database API",
            desc: "As a part of my role under SimplyPrint I've helped to further the 3D printing space, one of these projects lead me to help develop the Open Filament Database and it's accompanying website(s).",
            link_path: "https://api.openfilamentdatabase.org/",
            source_path: "https://github.com/OpenFilamentCollective/open-filament-database/",
            logo_path: "/sites/ofd.png"
        }
    ];
</script>

<svelte:head>
    <title>Websites</title>
</svelte:head>

<div class="w-full flex flex-col mb-5">
    <div class="mb-2 border-3 p-2 rounded-xl">
        <h1 class="text-3xl">Web Dev!</h1>
        <p>Besides working on Games I sometimes end up doing web dev, below are the websites I have either created or taken part in.</p>
    </div>

    <div class="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 xl:grid-cols-5 w-full gap-4 border-3 border-text p-2 rounded-xl">
        {#each sites as site}
            <SitePiece
                {site}
            />
        {/each}
    </div>
</div>
