<script lang="ts">
	import './layout.css';
	import favicon from '$lib/assets/favicon.svg';
	import Navbar from '$lib/components/navbar.svelte';

	let { children } = $props();
</script>

<svelte:head>
	<link rel="icon" href={favicon} />
</svelte:head>

<Navbar />

<div 
	class="mt-2.5 flex justify-center overflow-hidden /*And now our defaults*/ text-base"
	style="max-width: var(--body-width);">
	{@render children()}
</div>