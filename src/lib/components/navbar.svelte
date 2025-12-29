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
            name: 'Electronics',
            href: '/electronics',
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
            name: 'CV',
            href: '/cv',
            icon: null
        },
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

    const personalItems: MenuItem[] = [
        {
            name: 'About',
            href: '/about',
            icon: null
        },
        {
            name: 'Wishlist',
            href: '/wishlist',
            icon: null
        }
    ];
</script>

<div class="w-full flex justify-center">
    <div class="bg-mantle-bg text-surface p-2 rounded-b-xl flex justify-center flex-wrap gap-2">
        <ul class="bg-crust-bg overflow-hidden border rounded-xl flex justify-center flex-wrap">
            {#each projectItems as item}
                <NavButton item={item} />
            {/each}
        </ul>

        <ul class="bg-crust-bg overflow-hidden border rounded-xl flex justify-center flex-wrap">
            {#each profItems as item}
                <NavButton item={item} />
            {/each}
        </ul>

        <ul class="bg-crust-bg overflow-hidden border rounded-xl flex justify-center flex-wrap">
            {#each personalItems as item}
                <NavButton item={item} />
            {/each}
        </ul>

        {#if children}
            {@render children()}
        {/if}
    </div>
</div>