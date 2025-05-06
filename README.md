## INICIALIZACIÓN DE GIT ##

git config --global user.name "Tu Nombre de Git"
git config --global user.email "tu@email.com de Git"
----------------------------------------------------------------------
## Claves SSH ##

# Inciar agente SSH
eval "$(ssh-agent -s)"

# Moverse al directorio correctopara crear la clave
cd ~/.ssh

# Crear clave ssh
ssh-keygen

# Añadir la clave privada como identificador del dispositivo
ssh-add ~/.ssh/id_ed25519

# Imprimir la clave pública
cat ~/.ssh/clavePublica.pub

#copiar calve y meter en el usuario de gitHub
Perfil >> SSH and GPG Keys >> New SSH key >> Add SSH key
----------------------------------------------------------------------
## Comandos git ##
git clone
# Cambiarse de rama
git checkout main
# Entrar en nuestro proyecto del repositorio
cd myproject

# Añadir cambios
git add .

# Hacer commit
git commit -m "Bug arreglado"

# Hacer pull a una rama
git pull origin main

# Hacer push a una rama
git push origin main
