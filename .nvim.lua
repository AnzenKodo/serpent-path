-- ak: Build Initialization
-- ============================================================================

local build_dir = "build"
local build_command = ""
local cc_command = ""
build_command = "clang++ -ggdb build.cpp"
cc_command = "./a.out "

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

vim.keymap.set("n", "<F5>", "<CMD>BuildRun<CR>", { desc = "Build Run" })
