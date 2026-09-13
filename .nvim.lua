-- ak: Build Initialization
-- ============================================================================

local build_dir = "build"
local build_command = ""
local cc_command = ""
if (vim.fn.has('win32') == 1 or vim.fn.has('win64') == 1) then
    build_command = "setup_x64.bat && cl.exe build.c -nologo -Z7 -Fo:build\\ -Fe:"..build_dir.."\\a.exe"
    cc_command = "a.exe "
else
    build_command = "clang -ggdb build.c"
    cc_command = "./a.out "
end

-- ak: Build user commands
-- ============================================================================

-- NOTE(ak): run with `:make` below command
vim.opt.makeprg = cc_command .. "build-dry"
vim.api.nvim_create_user_command("BuildRun",  function()
    vim.opt.makeprg = cc_command .. "build-run"
    vim.cmd('make')
    vim.opt.makeprg = cc_command .. "build-dry"
end, { desc = "Bootstrap build system"})
vim.api.nvim_create_user_command("BuildBuild",  function()
    vim.opt.makeprg = build_command
    vim.cmd('make')
    vim.opt.makeprg = cc_command .. "build-dry"
end, { desc = "Bootstrap build system"})
vim.api.nvim_create_user_command("BuildMeta",  function()
    vim.opt.makeprg = cc_command .. "gen-meta"
    vim.cmd('make')
    vim.opt.makeprg = cc_command .. "build-dry"
    local success = vim.v.shell_error == 0
    if (success) then
        vim.cmd('make')
    end
end, { desc = "Bootstrap build system"})

-- ak: Setup Termdebug
-- ============================================================================

local program_name_debug = build_dir.."/cope_debug"
local termdebug_config = vim.g.termdebug_config or {}
termdebug_config.command = {
    "gdb", "-nx",
    "-ex", "set breakpoint pending on",
    "-ex", "set disassembly-flavor intel",
    "-ex", "set confirm off",
    "-ex", "set print pretty on",
    program_name_debug
}
vim.g.termdebug_config = termdebug_config

vim.api.nvim_create_autocmd("User", {
    pattern = "TermdebugStartPost",
    callback = function()
        vim.keymap.set("n", "<leader>dr", ":call TermDebugSendCommand('dr')<CR>", { desc = "[d]ebugger [r]un" })
        vim.cmd('cexpr system("' .. cc_command .. 'build-debugger --nocolor")')
        vim.fn.TermDebugSendCommand('define dr')
        vim.fn.TermDebugSendCommand('   shell '..cc_command.."build-debugger")
        vim.fn.TermDebugSendCommand('   file '..program_name_debug)
        vim.fn.TermDebugSendCommand('   run')
        vim.fn.TermDebugSendCommand('end')
    end,
})

vim.keymap.set("n", "<F5>", "<CMD>BuildRun<CR>", { desc = "Build Run" })
