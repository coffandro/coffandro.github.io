<script lang="ts">
	import './layout.css';
	import favicon from '$lib/assets/favicon.svg';
	import Navbar from '$lib/components/navbar.svelte';
	import { browser } from '$app/environment';

	let { children } = $props();
</script>

<svelte:head>
	<link rel="icon" href={favicon} />
</svelte:head>

<div class="absolute left-1/2 -translate-x-1/2">
	<Navbar />

	<div class="mt-2.5 flex justify-center overflow-hidden /*And now our defaults*/ text-base"
		style="width: var(--body-width);">
		{@render children()}
	</div>
</div>