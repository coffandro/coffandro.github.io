<script lang="ts">
    export let title: string, imagePath: string, classes: string = "";
</script>

<button command="show-modal" commandfor="dialog" class="p-2 bg-mantle-bg hover:bg-crust-bg rounded-lg flex flex-col justify-between cursor-pointer {classes}">
    <p class="text-center mb-2">{title}</p>
    <div class="aspect-square flex justify-center items-center">
        <img src={imagePath} class="rounded-lg">
    </div>
</button>

<el-dialog class="absolute /*This won't affect it, but flex ignores it*/">
    <dialog id="dialog" aria-labelledby="dialog-title" class="fixed inset-0 size-auto max-h-none max-w-none overflow-y-auto bg-transparent backdrop:bg-transparent">
    <el-dialog-backdrop class="fixed inset-0 bg-gray-900/50 transition-opacity data-closed:opacity-0 data-enter:duration-300 data-enter:ease-out data-leave:duration-200 data-leave:ease-in"></el-dialog-backdrop>

        <div tabindex="-1" class="flex min-h-full justify-center p-4 text-center focus:outline-none items-center sm:p-0">
            <el-dialog-panel class="relative transform overflow-hidden rounded-lg bg-gray-800 text-left shadow-xl outline -outline-offset-1 outline-white/10 transition-all data-closed:translate-y-4 data-closed:opacity-0 data-enter:duration-300 data-enter:ease-out data-leave:duration-200 data-leave:ease-in m-4 w-full data-closed:sm:translate-y-0 data-closed:sm:scale-95">
                <div class="bg-gray-800 px-4 pt-5 pb-4 sm:p-6 sm:pb-4">
                    <div class="sm:flex sm:items-start">
                        <div class="mt-3 text-center sm:mt-0 sm:ml-4 sm:text-left">
                            <div class="mt-2">
                                <img src={imagePath} class="rounded-lg">
                            </div>
                        </div>
                    </div>
                </div>
                <div class="bg-gray-700/25 px-4 py-3 flex justify-between">
                    <h3 id="dialog-title" class="text-base font-semibold text-xl text-white">{title}</h3>
                    <button type="button" command="close" commandfor="dialog" class="flex flex-col content-center justify-center rounded-md bg-gray-500 text-sm font-semibold text-white hover:bg-gray-400 ml-3 p-3 w-auto">Close</button>
                </div>
            </el-dialog-panel>
        </div>
    </dialog>
</el-dialog>