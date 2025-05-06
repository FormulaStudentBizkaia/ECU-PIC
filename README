## INICIALIZACIÓN DE GIT ##

git config --global user.name "Tu Nombre de Git"
git config --global user.email "tu@email.com de Git"

## Claves SSH
#Inciar agente SSH
eval "$(ssh-agent -s)"

# moverse al directorio correctopara crear la clave
cd ~/.ssh

#crear clave ssh
ssh-keygen 

ssh-add ~/.ssh/id_ed25519

cat ~/.ssh/clavePublica.pub
#copiar calve y meter en el usuario de gitHub
Perfil >> SSH and GPG Keys >> New SSH key >> Add SSH key

# Comandos git
#
git clone
# cambiarse de rama
git checkout main
# entrar en nuestro proyecto del repositorio
cd myproject

git add .
git commit -m "Bug arreglado"
git pull origin main
git push origin main
