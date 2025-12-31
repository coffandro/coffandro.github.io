<script lang="ts">
    let { children = null } = $props();
    import { type Icon as IconType } from '@lucide/svelte';
    import NavButton from './nav-button.svelte';

    type MenuItem = {
        name: string;
        href: string;
        icon: typeof IconType|null;
    };

    const projectItems: MenuItem[] = [
        {
            name: 'Home',
            href: '/',
            icon: null
        },
        {
            name: 'Art',
            href: '/art',
            icon: null
        }
    ];

    const profItems: MenuItem[] = [
        {
            name: 'Portfolio',
            href: '/portfolio',
            icon: null
        },
        {
            name: 'Games',
            href: '/games',
            icon: null
        },
        {
            name: 'Web dev',
            href: '/web',
            icon: null
        }
    ];
</script>

<div class="bg-mantle-bg text-surface p-2 rounded-b-xl flex justify-center flex-wrap gap-2">
    <ul class="bg-crust-bg overflow-hidden border rounded-xl flex grow justify-center flex-wrap">
        {#each projectItems as item}
            <NavButton item={item} />
        {/each}
    </ul>

    <ul class="bg-crust-bg overflow-hidden border rounded-xl flex grow justify-center flex-wrap">
        {#each profItems as item}
            <NavButton item={item} />
        {/each}
    </ul>

    {#if children}
        {@render children()}
    {/if}
</div>
