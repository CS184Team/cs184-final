# pushes to "origin writeup"
# you might need to modify that ^ on line 13
if [[ $# -eq 0 ]] ; then
    echo 'Pass in a message for the commit!:'
    echo 'Ex)'
    echo "'""\"Commit Message\"""'"
    exit 0
fi
set -e
git add -u
# git status
git commit -m $1
git push origin HEAD:writeup
# git status