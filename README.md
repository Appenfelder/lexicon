 For an overview of the project including links to downloads, documentation and a screenshot, see [here](http://laneslexicon.github.io).

Users may wish to check [here](https://github.com/laneslexicon/LexiconDatabase) for new versions of the database. The lastest version is as of 10th April 2018 (version 1.0.9).

This is the gui application repository for the project.

Windows and OSX users can download binaries from [here](https://github.com/laneslexicon/lexicon/releases).


Linux and *BSD users will need to build the application. The quickest way to build is as below. You will need to install Qt and LibXSLT. (On Debian based systems, the packages are libxml2-dev and libxlst1-dev.)


```
# fetch the sources
> git clone https://github.com/laneslexicon/lexicon
> cd lexicon
# generate the make file
> qmake laneslexicon.pro
# build it. On FreeBSD this is 'gmake'
> make
# fetch the database, images, documentation etc
>git clone https://github.com/laneslexicon/lexicon_config
# give it the right name
>mv lexicon_config Resources
>cd Resources
>unzip lexicon.sqlite.zip
>cd ..
## run the application
>./laneslexicon
```

Please see [here](http://laneslexicon.github.io/lexicon/site/dev-guide/build/index.html) for a description and for more detailed build instructions.
