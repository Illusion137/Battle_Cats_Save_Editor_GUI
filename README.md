# Battle Cats Save Editor GUI
NOT FINISHED DO NOT USE YET

This is a WIP GUI version of [fieryhenry's](https://github.com/fieryhenry) own [Python Battle Cats Save File Editor](https://github.com/fieryhenry/BCSFE-Python)

Dependencies :
- [boost](https://github.com/boostorg/boost) ( boost's regex library due to improved speeds compared to STL )
- [GLFW](https://github.com/glfw/glfw) ( backend renderer for Dear ImGUI )
- [Dear ImGUI](https://github.com/ocornut/imgui) ( GUI library for C++ )
- [nlohmann/json](https://github.com/nlohmann/json) ( JSON library; planning on migrating to [RapidJSON](https://github.com/Tencent/rapidjson) )

Current Progress :
- Collected what I believe is all necessary data for Battle Cats to use in the Save Editor
- Setup a NodeJS webscraper to fetch as much data as I can for each update ( not in repository )
- GUI and custom widgets to make the apperance of the Editor to be similar to Battle Cat's GUI
- Getting Data from JSON 'SAVE_DATA' file
- Cat Editor
  - Edit Cat Level, Plus-Level, Lock/Unlock, Form
  - (WIP) Batch Edit Cats
  - Undo/Redo Edits
  - Search by cat name/IDs
  - (WIP) Search by Cat Filters(Settings, Rarity, Target, Effect/Ability)
  - Save Data back to 'SAVE_DATA' file

As a WIP project I still found use in this project by using the WIP Cat Filters to find which cats to edit in on [fieryhenry's](https://github.com/fieryhenry) [Battle Cats Save File Editor](https://github.com/fieryhenry/BCSFE-Python)
