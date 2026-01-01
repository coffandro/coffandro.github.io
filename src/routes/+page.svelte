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

<div class="w-full flex flex-col mb-10">
    <div class="flex">
        <div class="w-2/3">
            <h1 class="text-3xl">Welcome to my website!</h1>
            <p>My name's Frida and I'm a Game Developer and Software developer from Denmark</p>
        </div>
        <div class="relative w-1/3 aspect-square overflow-hidden border border-text rounded-full">
            <img src="/Frida.png" class="absolute -left-2 top-2.5">
        </div>
    </div>

    <div class="circles-container w-full aspect-square border-3 border-text rounded-xl">
        <div class="outer-circle">
            {#each links as link, index}
                <div
                    class="animation{index+1} rotate"
                    data-animation
                >
                    <a
                        bind:this={link.element}
                        href={link.href}
                        class="inner text-lg sm:text-2xl border-2 border-text hover:bg-hover"
                    >
                        {link.name}
                    </a>
                </div>
            {/each}
        </div>
    </div>
</div>

<style>
    .circles-container {
        position: relative;
        margin: 0 auto;
        top: calc(var(--spacing) * 5);
        --circle-offset: 75%;
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