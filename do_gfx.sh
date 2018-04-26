echo --- GFX ---
PROJECT_FOLDER=car_game
RES_FOLDER=$PROJECT_FOLDER/src/res
GFX_FOLDER=gfx
TOOLS_FOLDER=tools
COPYRIGHT_FILE=$TOOLS_FOLDER/copyright.inc

python $TOOLS_FOLDER/bin2c.py -i $GFX_FOLDER/environment.abs -o $RES_FOLDER/env_sprite.h -m 8 -c $COPYRIGHT_FILE
python $TOOLS_FOLDER/bin2c.py -i $GFX_FOLDER/car.abs -o $RES_FOLDER/car_sprite.h -m 8 -c $COPYRIGHT_FILE
python $TOOLS_FOLDER/bin2c.py -i $GFX_FOLDER/font.abs -o $RES_FOLDER/font.h -m 8 -c $COPYRIGHT_FILE
python $TOOLS_FOLDER/fontmap.py -i $GFX_FOLDER/fontmap.txt -o $RES_FOLDER/fontmap.h -c $COPYRIGHT_FILE

echo Done
