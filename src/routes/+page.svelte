<script lang="ts">
    type Link = {
        name: string;
        href: string;
        element: HTMLElement|null;
    };

    let links: Link[] = [
        {
            name: "Web dev",
            href: "/web",
            element: null
        },
        {
            name: "Art",
            href: "/art",
            element: null
        },
        {
            name: "Games",
            href: "/games",
            element: null
        }
    ];
</script>

<svelte:head>
    <title>Coffandro's website</title>
</svelte:head>

<div class="grow mb-2 gap-3 flex flex-col-reverse sm:flex-row">
    <div class="grow flex flex-col min-h-0 overflow-y-hidden overflow-x-hidden">
        <div class="border-3 rounded-xl p-3 mb-2">
            <h1 class="text-5xl">Frida Rosenaa</h1>
            <p class="text-xl">I make games, websites, electronics and scripts to tie it all together.</p>
            <p class="text-xl">I've been programming since 2019, making electronics since 2021, making games and websites since 2022.</p>
        </div>

        <div class="grow border-3 rounded-xl flex align items-center justify-center overflox-hidden">
            <div class="circles-container aspect-square h-full">
                <div class="outer-circle">
                    {#each links as link, index}
                        <div
                            class="animation{index+1} rotate"
                            data-animation
                        >
                            <a
                                bind:this={link.element}
                                href={link.href}
                                data-sveltekit-reload
                                class="inner text-lg sm:text-2xl border-3 hover:bg-hover"
                            >
                                {link.name}
                            </a>
                        </div>
                    {/each}
                </div>
            </div>
        </div>
    </div>

    <div class="flex-col gap-2 hidden sm:flex">
        <div class="relative rounded-xl overflow-hidden border-3">
            <img src="/Frida.png" class="mx-auto w-full">
        </div>

        <!-- This makes our socials appear on the side on PC, part of the header section on mobile-->
        <div class="relative rounded-xl overflow-hidden border-3 grow p-3">
            <h2 class="text-3xl">Socials</h2>
        </div>
    </div>
</div>

<style>
    .circles-container {
        position: relative;
        --circle-offset: 85%;
    }

    .outer-circle {
        position: absolute;
        left: 50%;
        top: 50%;
        transform: translate(-50%, -50%);
        width: 35%;
        height: 35%;
        border-radius: 50%;
    }

    .rotate {
        width: 100%;
        height: 100%;
        position: absolute;
    }

    .inner {
        display: flex;
        align-items: center;
        justify-content: center;
        width: 100%;
        aspect-ratio: 1/1;
        border-radius: 50%;
    }
    
    .animation1 {
        animation-name: circle1;
    }
    .animation2 {
        animation-name: circle2;
    }
    .animation3 {
        animation-name: circle3;
    }

    .animation1,
    .animation2,
    .animation3 {
        animation-duration: 45s;
        animation-iteration-count: infinite;
        animation-timing-function: linear;
    }

    @keyframes circle1 {
        from {
            transform: rotateZ(0deg) translate(var(--circle-offset)) rotateZ(-0deg);
        }
        to {
            transform: rotateZ(360deg) translate(var(--circle-offset)) rotateZ(-360deg);
        }
    }

    @keyframes circle2 {
        from {
            transform: rotateZ(120deg) translate(var(--circle-offset)) rotateZ(-120deg);
        }
        to {
            transform: rotateZ(480deg) translate(var(--circle-offset)) rotateZ(-480deg);
        }
    }

    @keyframes circle3 {
        from {
            transform: rotateZ(240deg) translate(var(--circle-offset)) rotateZ(-240deg);
        }
        to {
            transform: rotateZ(600deg) translate(var(--circle-offset)) rotateZ(-600deg);
        }
    }
</style>