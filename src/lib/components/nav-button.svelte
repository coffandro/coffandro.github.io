<script lang="ts">
    import { type Icon as IconType } from "@lucide/svelte"

    type MenuItem = {
        name: string;
        href: string;
        icon: typeof IconType|null;
    }

    export let item: MenuItem; 
</script>

{#if item}
    {@const Icon = item.icon}
    <li 
        class="bg-surface-bg hover:bg-base-bg not-last:border-r px-1 text-nowrap grow"
    >
        <a 
            href={item.href}
            class="block p-1 text-lg"
        >
            {#if Icon}
                <Icon />
            {/if}
            {item.name}
        </a>
    </li>
{/if}