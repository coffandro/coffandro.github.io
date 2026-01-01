<script lang="ts">
    import GamePiece from "$lib/components/games/game-piece.svelte";
    import type { Game } from "$lib/game";

    const games: Game[] = [
        {
            title: "Fallout: London",
            desc: "A game sized overhaul mod for Fallout 4 with a new story, new weapons, London-centric items and a vast recreation of the ruined city to explore.",
            link_path: "https://fallout4london.com/",
            youtupe_path: "https://www.youtube.com/embed/rahBCfRdVac?si=NiOcjyH3Qe23b9u3",
            logo_path: "/games/fallout_london.png",
        },
        {
            title: "Fight for the Forest",
            desc: "An award winning game built for a coop arcade experience as a school project. In this game you and a friend fight who gets to control the forest using projectile carrots, fists and powerups",
            link_path: "https://coffandro.itch.io/fightforforest",
            source_path: "https://github.com/coffandro/ArcadeGame",
            youtupe_path: "https://www.youtube.com/embed/vzFcYm8tkD4?si=xy6S1VtoEHaTeDiH",
            logo_path: "/games/fight_for_forest.png",
            itch_path: "https://itch.io/embed-upload/12607860?color=ecc8c3",
        },
        {
            title: "Fishies",
            desc: "My first game, built to learn 3D, Godot and just about anything else. You are a fish and your job is to jump on the other fish, kill as many as you can before the end.",
            link_path: "https://coffandro.itch.io/fishies",
            logo_path: "/games/fishies.png",
            image_path: "/games/fishies_screenshot.png",
            itch_path: "https://itch.io/embed-upload/9917750?color=333333",
        },
        {
            title: "KATastrofe",
            desc: `In collaboration with Save the child I helped develop a board game to promote child safety on the internet.`,
            link_path: "https://katastrofespil.github.io/",
            source_path: "https://github.com/KatastrofeSpil/KatastrofeSpil.github.io",
            logo_path: "/games/katastrofe.svg",
        },
        {
            title: "Tiles In Progress",
            desc: `Have a platformer! we did get lazy with the level design so that's up to you :/`,
            link_path: "https://coffandro.itch.io/tiles-in-progress",
            source_path: "https://github.com/BreadBirdGames/Tiles-In-Progress",
            logo_path: "/games/tiles_in_progress.png",
            itch_path: "https://itch.io/embed-upload/13918277?color=4aa9e4",
        },
        {
            title: "SPC",
            desc: `A little SPaCe game where you use shock waves to kill alien blobs, press X to fire your weapon and rotate using the arrows.`,
            link_path: "https://coffandro.itch.io/spc",
            logo_path: "/games/spc.png",
            itch_path: "https://itch.io/embed-upload/10772133?color=333333",
        },
        {
            title: "Meeleetonin",
            desc: `Meleetonin is about Jimmy and his struggles with nightmares, you need to help him take his plushies and pillow to beat them up.`,
            link_path: "https://coffandro.itch.io/meleetonin",
            source_path: "https://github.com/BreadBirdGames/FiveNightSleep",
            logo_path: "/games/meeleetonin.png",
            image_path: "/games/meeleetonin_screenshot.png",
            itch_path: "https://itch.io/embed-upload/13293436?color=2a4c90",
        },
        {
            title: "Round Snake",
            desc: `A game built of burnout, 8 directional snake.`,
            link_path: "https://coffandro.itch.io/round-snake",
            logo_path: "/games/round_snake.png",
            itch_path: "https://itch.io/embed-upload/11323070?color=000000",
        },
        {
            title: "One Last Straw",
            desc: `A clicker style prototype built around promoting pro environment action.`,
            link_path: "https://coffandro.itch.io/one-last-straw",
            source_path: "https://github.com/coffandro/NEXT-Cup_straws",
            logo_path: "/games/one_last_straw.png",
            itch_path: "https://itch.io/embed-upload/15451395?color=333333",
        },
        {
            title: "Switcheroo",
            desc: `A game about switching characters and platforming by BreadBird Games.`,
            link_path: "https://coffandro.itch.io/switcheroo",
            source_path: "https://github.com/BreadBirdGames/Switcheroo",
            logo_path: "/games/switcheroo.png",
            itch_path: "https://itch.io/embed-upload/13158789?color=333333",
        },
        {
            title: "Ur",
            desc: `A failed remake of the 4000 year old Ur board game.`,
            link_path: "https://coffandro.itch.io/ur",
            source_path: "https://github.com/coffandro/Ur",
            logo_path: "/games/ur.png",
            itch_path: "https://itch.io/embed-upload/11231067?color=333333",
        },
        {
            title: "Whack A Capitalist",
            desc: `A game built for the Red Green Youth party in the wake of the 2024 election results where you whack capitalists.`,
            link_path: "https://coffandro.itch.io/wack-a-capitalist",
            logo_path: "/games/whacka.png",
            itch_path: "https://itch.io/embed-upload/11935166?color=ff0000",
        },
        {
            title: "Krimi",
            desc: `Krimi is a little experiment I made to test out a dialogue system about waking up after night out`,
            link_path: "https://coffandro.itch.io/krimi",
            source_path: "https://github.com/coffandro/Krimi",
            logo_path: "/games/krimi.png"
        },
        {
            title: "Wizards",
            desc: `My first "big" game, a Enter the Gungeon style game in 3D, it didn't get past it's rough prototype sadly`,
            link_path: "https://coffandro.itch.io/wizards",
            source_path: "https://github.com/coffandro/Wizards",
            logo_path: "/games/wizards.png",
            itch_path: "https://itch.io/embed-upload/9917733?color=333333",
        },
        {
            title: "OSO Invaders",
            desc: `An exam project Space Invaders clone `,
            link_path: "https://coffandro.itch.io/oso-invaders",
            logo_path: "/games/oso_invaders.png",
        },
        {
            title: "Gnomes",
            desc: `The rought prototype of a game originating from my dreams where you shoot gnomes trying to take your castle`,
            link_path: "https://coffandro.itch.io/gnomes",
            source_path: "https://github.com/coffandro/Gnomes",
            logo_path: "/games/gnomes.png"
        },
    ];
</script>

<svelte:head>
    <title>Games</title>
</svelte:head>

<div class="w-full flex flex-col mb-5">
    <div class="mb-2">
        <h1 class="text-3xl">Games!</h1>
        <p>These are the games which I have either made on my own or worked on, the list is ever expanding, sorted based upon which games I'm the most proud of, not success or date of publishing.</p>
    </div>

    <div class="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 xl:grid-cols-5 w-full gap-4">
        {#each games as game}
            <GamePiece
                {game}
            />
        {/each}
    </div>
</div>
