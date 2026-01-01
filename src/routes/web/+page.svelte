<script lang="ts">
    import SitePiece from "$lib/components/sites/site-piece.svelte";
    import type { Site } from "$lib/site";

    const sites: Site[] = [
        {
            title: "Team FOLON website",
            desc: "As a favor for the FOLON team I helped in building a new and improved website to show off their brand",
            link_path: "https://www.teamfolon.com/",
            logo_path: "/sites/team_folon.png"
        },
        {
            title: "SimplyPrint",
            desc: "For X years I worked as a Junior Software developer for SimplyPrint, working on features such as the initial student dashboard feature and a printer integration.",
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
            desc: "In collaboration with Save the child I helped develop a board game to promote child safety on the internet, and the accompanying website.",
            link_path: "https://katastrofespil.github.io/",
            source_path: "https://github.com/KatastrofeSpil/KatastrofeSpil.github.io",
            logo_path: "/sites/katastrofe.svg"
        },
        {
            title: "Gallery",
            desc: "A small UI mockup I created for an imaginary artist.",
            link_path: "http://coffandro.dk/Gallery/",
            source_path: "https://github.com/coffandro/Gallery",
            logo_path: "/sites/gallery.svg"
        }
    ];
</script>

<svelte:head>
    <title>Sites</title>
</svelte:head>

<div class="w-full flex flex-col mb-5">
    <div class="mb-2">
        <h1 class="text-3xl">Web Dev!</h1>
        <p>Besides working on Games I sometimes end up doing web dev, below are the websites I have either created or taken part in.</p>
    </div>

    <div class="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 xl:grid-cols-5 w-full gap-4">
        {#each sites as site}
            <SitePiece
                {site}
            />
        {/each}
    </div>
</div>
