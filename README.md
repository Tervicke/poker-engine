# poker-engine
A simple poker engine written in c++ and gui is powered by SFML , the engine is very simple , for every stage of the game it runs 100k trials simulating the remaining cards and getting a win and loss % over the trails .

![screenshot](screenshot.png)
# Build and run 
before you build make sure u have the sfml and tinyxml2 installed.

to build the binary 
```
    make build
```

to run the binary 
```
    cd build
    ./poker
```

Press the `spacebar` to progress in the game.  

# TODO
- [x] Enable calculation pre FLOP stage
- [x] Option to Restart engine
- [ ] Add option to save and load custom game state.   
