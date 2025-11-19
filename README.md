

# Grid 5000  

## Config ssh  

**Sous linux**
```conf
Host g5k
        User uvallat
        Hostname access.grid5000.fr
        IdentityFile ~/.ssh/grid5000
        AddKeysToAgent yes
        ForwardAgent no

Host *.g5k
        User uvallat
        ProxyCommand ssh g5k -W "$(basename %h .g5k):%p"
        ForwardAgent no
```

**Sous Windows**
```conf
Host g5k
    User uvallat
    HostName access.grid5000.fr
    IdentityFile ~/.ssh/grid5000
    AddKeysToAgent yes
    ForwardAgent no

Host *.g5k
    User uvallat
    # Remplace ProxyJump par une ProxyCommand PowerShell qui fait: dest = %h sans ".g5k" + ".grid5000.fr"
    ProxyCommand powershell -NoProfile -Command "$h='%h'; $p='%p'; $dest=($h -replace '\.g5k$','') + '.grid5000.fr'; ssh -q g5k -W \"$dest`:$p\""
    ForwardAgent no
```

```sh
ssh -J <ville>.g5k uvallat@<cluster>-<machine>
```

## Clé d'accès à grid5000

**Clé publique**
```
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIB7+xPxAL80dXrXppMZnMNzcSrj3nMO4vYpd0UuYC2yq ugo-linux@LAPTOP-032VBVCP
```

**Clé privée**

## Clé d'accès au dépôt git depuis grid5000  

```
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIE0Dz90fse91oyF4bCkdQ4Hbh7Sf2vayAouYss5j7ON9 ugo-linux@LAPTOP-032VBVCP
```


## Config du serveur

```sh

git_key_pub="ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIE0Dz90fse91oyF4bCkdQ4Hbh7Sf2vayAouYss5j7ON9 ugo-linux@LAPTOP-032VBVCP"


echo "$git_key" >> .ssh/github
echo "$git_key_pub" >> .ssh/github.pub
chmod 600 .ssh/github

eval "$(ssh-agent -s)"
ssh-add ~/.ssh/github

git clone git@github.com:ugo-vallat/M2-S9-SECIL-SE-ENERGIE.git se-energie 

git config --global alias.lol "log --graph --decorate --pretty=oneline --abbrev-commit"
git config --global alias.lola "log --graph --decorate --pretty=oneline --abbrev-commit --all"

git config --global user.name "Ugo VALLAT"
git config --global user.email "ugo.vallat@univ-tlse3.fr"  
git config --global core.editor "vim"


```



```.bashrc
echo "Run .bashrc"

alias sudo="sudo-g5k"
alias gits="git status"
alias ls='ls --color=auto'
alias ll="ls -la"


# Init
sudo cp /home/uvallat/mojitos/bin/mojitos /usr/bin/
sudo sh -c 'echo 0 >/proc/sys/kernel/perf_event_paranoid'
sudo chmod o+r /sys/class/powercap/intel-rapl/*/*
sudo chmod o+r /sys/class/powercap/intel-rapl/*/*/*
```

# Résultat  

## Res 1
Algo = tri python (5_000_000 éléments)
Fréquence échatillonage = 100Hz

Freq (Hz) | Duration (s) | Energy (Joules) | Pmax (W)
|---|---|---|---|
1000000|14.422047488|437.603902|51.719000
1100000|12.971675136|397.051655|54.132100
1200000|11.951496960|369.043763|51.539200
1300000|11.053574144|341.340869|49.176800
1400000|10.275834112|318.154424|53.323500
1500000|9.656436480|305.400215|49.581700
1600000|9.091219968|293.425680|50.329700
1700000|8.527257344|273.152330|53.388100
1800000|8.200987904|265.930859|51.918100
1900000|7.749977600|251.993465|51.053000
2000000|7.409659904|241.341386|56.501700
2100000|7.003344384|228.250516|50.793900
2200000|6.622208768|308.970459|69.603000
2300000|6.368577792|297.644720|54.468000
2400000|6.138954496|288.955332|76.708500
2500000|5.854084608|277.468967|55.690200
2600000|5.661284864|269.726681|58.037500
2700000|5.483106560|271.688011|82.505600
2800000|5.273861120|256.646673|57.863600
2900000|5.167610624|252.983811|59.191600
3000000|4.917712896|245.454391|77.154900
3100000|4.784040960|241.526493|79.576200
3200000|4.669434880|236.874078|61.481000
3300000|4.752097792|245.107911|79.473500
3400000|4.457057792|231.084422|64.984700
3500000|4.298038016|225.597116|66.869600
3600000|4.233787904|225.594512|80.054900
3700000|4.117557504|220.880190|68.560900
3800000|4.033497600|216.949327|66.229400



# Ressources  

git travail : `git@github.com:ugo-vallat/M2-S9-SECIL-SE-ENERGIE.git`
Sujet TP : `https://www.irit.fr/~Georges.Da-Costa/syst%C3%A8mes-embarqu%C3%A9s-%C3%A9nergie/`
