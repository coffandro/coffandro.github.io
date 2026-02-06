<script lang="ts">
    import { type Icon as IconType } from "@lucide/svelte"
    import { page } from "$app/state";

    type MenuItem = {
        name: string;
        href: string;
        icon: typeof IconType|null;
    }

    export let item: MenuItem;

    function isActive(target: string) {
        if (!target.includes("/")) {
            target = "/" + target;
        }

        return page.url.pathname == target;
    }
</script>

{#if item}
    {@const Icon = item.icon}
    <li 
        class="hover:bg-hover not-last:border-r grow {isActive(item.href) ? 'bg-focus' : ''}"
        data-sveltekit-reload
    >
        <a 
            href={item.href}
            class="p-1 text-lg flex justify-center sm:block"
        >
            {#if Icon}
                <Icon />
            {/if}
            {item.name}
        </a>
    </li>
{/if}