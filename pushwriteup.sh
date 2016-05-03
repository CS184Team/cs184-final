# pushes to "origin writeup"
# you might need to modify that ^ on line 20
if [[ $# -eq 0 ]] ; then
    echo 'Pass in a message for the commit!:'
    echo 'Ex) -- (No Quotes Needed)'
    echo "Commit Message"
    exit 0
fi

C=''
for i in "$@"; do 
    C="$C ${i//\"/\\\"}"
done

set -e
git add -u
# git status
git commit -m "$C"
git push origin HEAD:writeup
# git status