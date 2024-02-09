# ppSAT

This repository uses the work from Luo et al. - ppSAT Towards Two-Party Private SAT Solving.

**Improvements**

- Now using only one formula per party as input
- Drafted maliciously secure version
- Added docker container such that the project is installable


## Installing

1. Build the docker image contained in `main` with `docker build .` 
2. Create a docker container with `docker run -t -d -v $(pwd)/exec:/root DOCKERIMAGEID` **from the directory in which the dockerfile is in**
    - Replace DOCKERIMAGEID with the image id of the previously created image (`docker images`)
3. Start the container with `docker start ID`   
    - ID is the id of the docker container
4. Hook into the container with your console of choice (bash) from the directory in which the 
    `docker exec -u 0 -it <name-of-docker-container> bash`
5. Perform:

        sudo ldconfig
        cd /root
        wget https://raw.githubusercontent.com/emp-toolkit/emp-readme/master/scripts/install.py
        python3 install.py -install --tool --ot --sh2pc --ag2pc
        cd src
        wget https://raw.githubusercontent.com/emp-toolkit/emp-readme/master/scripts/install.py
        python3 install.py -install --tool --ot --sh2pc --ag2pc
        cmake . 
        make
        cd ..
        cmake .
        make



## Files of interest


`ppsat` in the /src directory comes from the latest commit from the authors. The ppsat.cpp file has changed compared to the original source code in that the command now uses two phi inputs instead of one. However, these inputs are now also being demanded by both parties. This results in the following (exemplary) usecase:

    ./ppsat 1 12345 2 5 "(1 2)(-1 -2)" "(-1 2)(1 -2)" & ./ppsat 2 12345 2 5 "(1 2)(-1 -2)" "(-1 2)(1 -2)"


Only providing one input will result in a segfault or other errors. This obviously defeats the purpose of *private* sat solving.
We have created a new version called `ppsat_2` which only requires one input from each party.



## Running the original ppSAT files

Evaluate solver on one step:
```shell
src/ppsat 1 12345  $nvar $nstep $phi # run this in one terminal
src/ppsat 2 12345  $nvar $nstep $phi # run this in another terminal
$nvar: the number of variables of the formulae
$nstep: the maximum number of giant steps that the solver will run. 
$phi: the string of the input formula. The syntax of the formula is the (\(-? [0-9]+\))+
```
