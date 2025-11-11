# Exexute this file with source command : "source .init_home"

# bashrc
cp .bashrc ~/.bashrc
source .bashrc

# git
git config --global user.name "Ugo VALLAT"
git config --global user.email "ugo.vallat@univ-tlse3.fr"  
git config --global core.editor "vim"
git config --global alias.lol "log --graph --decorate --pretty=oneline --abbrev-commit"
git config --global alias.lola "log --graph --decorate --pretty=oneline --abbrev-commit --all"
git config --global alias.subup "submodule update --init --recursive"
git pull
git subup




