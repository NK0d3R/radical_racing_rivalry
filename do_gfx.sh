echo --- GFX ---
PROJECT_FOLDER=car_game
GFX_FOLDER=gfx
TOOLS_FOLDER=tools
COPYRIGHT_FILE=$TOOLS_FOLDER/copyright.inc

python $TOOLS_FOLDER/bin2c.py -i $GFX_FOLDER/environment.abs -o $PROJECT_FOLDER/env_sprite.h -m 8 -c $COPYRIGHT_FILE
python $TOOLS_FOLDER/bin2c.py -i $GFX_FOLDER/car.abs -o $PROJECT_FOLDER/car_sprite.h -m 8 -c $COPYRIGHT_FILE
python $TOOLS_FOLDER/bin2c.py -i $GFX_FOLDER/font.abs -o $PROJECT_FOLDER/font.h -m 8 -c $COPYRIGHT_FILE
python $TOOLS_FOLDER/fontmap.py -i $GFX_FOLDER/fontmap.txt -o $PROJECT_FOLDER/fontmap.h -c $COPYRIGHT_FILE

echo Done
