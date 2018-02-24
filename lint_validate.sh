echo Checking source files
PROJECT_FOLDER=car_game
TOOLS_FOLDER=tools
GOOGLE_LINT=$TOOLS_FOLDER/third-party/cpplint/cpplint.py
for filename in $PROJECT_FOLDER/*; do
    OUT=`git diff $filename`
    if [ ! -z "$OUT" ]; then
        echo "$filename was updated, checking"
        while :
        do
            python $GOOGLE_LINT --repository="$PROJECT_FOLDER" $filename
            if [ $? == 0 ]; then
                echo File is OK !
                break
            else
                echo "Errors found ! (r)etry or (n)ext"
                read res
                if [ $res == "n" ]; then
                    break
                fi
            fi
        done
    fi
done
echo Done
