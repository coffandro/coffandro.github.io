<script lang="ts">
	import './layout.css';
	import favicon from '$lib/assets/favicon.svg';
	import Navbar from '$lib/components/navbar.svelte';

	let { children } = $props();
</script>

<svelte:head>
	<link rel="icon" href={favicon} />
</svelte:head>

<div>
	<Navbar />

	<div class="mt-2.5 px-2 flex justify-center overflow-hidden /*And now our defaults*/ text-base">
		{@render children()}
	</div>
</div>