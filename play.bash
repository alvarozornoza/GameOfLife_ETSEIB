if make ; then
    echo "Make succeeded"
    if [ "$#" -eq 0 ]; then
    ./conway
    elif [ "$#" -eq 1 ]; then 
        if [ -f $1 ]; then
            ./conway "$1"
        else
            echo "File $1 does not exist."
        fi
    else
        echo "Invalid number of parameters. Try again :)"
    fi
else
    echo "Make failed"
fi
