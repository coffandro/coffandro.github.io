<script lang="ts">
    import { LinkedinIcon, GithubIcon, InstagramIcon, type Icon as IconType } from "@lucide/svelte";
    import Fa from 'svelte-fa';
    import { faItchIo, type IconDefinition } from '@fortawesome/free-brands-svg-icons';

    type Social = {
        title: string;
        href: string;
        logo_url?: string;
        icon?: typeof IconType;
        fa_icon?: IconDefinition
    }

    export let classes: string = "", is_horizontal: boolean = false;

    let socials: Social[] = [
        {
            title: "LinkedIn",
            href: "https://www.linkedin.com/in/frida-rosenaa-901058291/",
            icon: LinkedinIcon
        },
        {
            title: "Github",
            href: "https://github.com/coffandro/",
            icon: GithubIcon
        },
        {
            title: "Instagram",
            href: "https://www.instagram.com/coffandro/",
            icon: InstagramIcon
        },
        {
            title: "Itch",
            href: "https://coffandro.itch.io/",
            fa_icon: faItchIo
        }
    ];
</script>

<div class="flex flex-wrap gap-2 {is_horizontal ? 'flex-row' : 'flex-col'} {classes}">
    {#each socials as item}
        {@const Icon = item.icon}
        <a href={item.href} class="border-2 p-1 hover:bg-hover rounded-xl flex grow flex-row {is_horizontal ? 'justify-center flex-wrap' : ''}">
            {#if Icon}
                <Icon />
            {:else if item.logo_url}
                <img src={item.logo_url}>
            {:else if item.fa_icon}
                <Fa icon={item.fa_icon} style="height: 1.5em;"/>
            {/if}

            {#if !is_horizontal}
                <span class="ml-2">{item.title}</span>
            {/if}
        </a>
    {/each}
</div>