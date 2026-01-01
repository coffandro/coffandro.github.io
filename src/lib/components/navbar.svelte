<script lang="ts">
    let { children = null } = $props();
    import { type Icon as IconType } from '@lucide/svelte';
    import NavButton from './nav-button.svelte';

    type MenuItem = {
        name: string;
        href: string;
        icon: typeof IconType|null;
    };

    type OptionalItem = {
        menu_item: MenuItem;
        key: string;
    }

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

    // A page has the abilty to add a localstorage item about being visited in which case it will show up in the navbar
    const optionalItems: OptionalItem[] = [
        
    ];

    let enabledOptionalItems: MenuItem[] = []
    if (typeof window !== 'undefined') {
        if (localStorage != null) {
            let enabledTabsString: string|null = localStorage.getItem("enabledTabs");
            if (enabledTabsString != null) {
                let enabledTabs = JSON.parse(enabledTabsString);

                optionalItems.forEach((item) => {
                    if (enabledTabs?.["items"].includes(item.key)) {
                        enabledOptionalItems.push(item.menu_item);
                    }
                });
            }
        }
    }
</script>

<div class="text-surface p-2 rounded-b-xl border border-t-0 mx-2 border-text flex justify-center flex-wrap gap-2">
    <ul class="overflow-hidden border rounded-xl flex grow justify-center flex-wrap">
        {#each projectItems as item}
            <NavButton item={item} />
        {/each}
    </ul>

    <ul class="overflow-hidden border rounded-xl flex grow justify-center flex-wrap">
        {#each profItems as item}
            <NavButton item={item} />
        {/each}
    </ul>

    {#if enabledOptionalItems.length > 0}
        <ul class="overflow-hidden border rounded-xl flex grow justify-center flex-wrap">
            {#each enabledOptionalItems as item}
                <NavButton item={item} />
            {/each}
        </ul>
    {/if}

    {#if children}
        {@render children()}
    {/if}
</div>
