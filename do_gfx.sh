echo --- GFX ---
PROJECT_FOLDER=car_game
GFX_FOLDER=gfx
TOOLS_FOLDER=tools
python $TOOLS_FOLDER/bin2c.py -i $GFX_FOLDER/environment.abs -o $PROJECT_FOLDER/env_sprite.h -m 8
echo Done