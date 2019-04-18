" Plugins
if has('nvim')
  let s:vim_dir = $HOME . '/.config/nvim'
else
  let s:vim_dir = $HOME . '/.vim'
endif

let s:plugin_dir = s:vim_dir . '/plugins'
let s:vimplug = s:vim_dir . '/autoload/plug.vim'
let s:vimplug_uri = 'https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim'

if empty(glob(s:vimplug))
  execute '!curl -fsSLo ' . s:vimplug . ' --create-dirs ' . s:vimplug_uri
endif

call plug#begin(s:plugin_dir)
  Plug 'neoclide/coc.nvim', {
  \   'do': { -> coc#util#install() }
  \ }
call plug#end()

" Auto-install missing plugins
autocmd VimEnter *
 \  if len(filter(values(g:plugs), '!isdirectory(v:val.dir)'))
 \|   PlugInstall --sync | q
 \| endif

" https://github.com/neoclide/coc.nvim#example-vim-configuration
set hidden
