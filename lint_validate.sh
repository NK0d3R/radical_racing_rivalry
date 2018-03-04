echo Checking source files
PROJECT_FOLDER=car_game
TOOLS_FOLDER=tools
GOOGLE_LINT=$TOOLS_FOLDER/third-party/cpplint/cpplint.py
for filename in $PROJECT_FOLDER/*; do
    echo "Checking $filename"
    while :
    do
        python $GOOGLE_LINT --root="$PROJECT_FOLDER"                    \
                --repository="$PROJECT_FOLDER" --filter=-build/include  \
                $filename
        if [ $? == 0 ]; then
            echo File is OK !
            break
        else
            echo "Errors found ! (r)etry (n)ext (q)uit"
            read res
            if [ $res == "n" ]; then
                break
            fi
            if [ $res == "q" ]; then
                exit 255
            fi
        fi
    done
done
echo Done
