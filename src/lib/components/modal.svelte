<script>
    export let children, topbar, id;
</script>

<el-dialog class="absolute /*This won't affect it, but flex ignores it*/">
    <dialog id="{id}-dialog" aria-labelledby="{id}-dialog-title" class="fixed inset-0 size-auto max-h-none max-w-none overflow-y-auto bg-transparent backdrop:bg-transparent">
    <el-dialog-backdrop class="fixed inset-0 bg-gray-900/50 data-closed:opacity-0"></el-dialog-backdrop>

        <div tabindex="-1" class="flex min-h-full justify-center p-4 text-center focus:outline-none items-center sm:p-0">
            <el-dialog-panel class="relative transform overflow-hidden rounded-lg bg-bg text-left shadow-xl m-4 w-full border-2 border-text data-closed:opacity-0">
                <div class="bg-gray-700/25 border-b text-text px-4 py-3 flex justify-between items-center">
                    {@render topbar()}
                </div>
                <div class="px-2 pt-3 p-3 pb-4">
                    <div class="flex items-start">
                        <div class="mt-3 w-full ml-4 text-text text-left">
                            {@render children()}
                        </div>
                    </div>
                </div>
            </el-dialog-panel>
        </div>
    </dialog>
</el-dialog>