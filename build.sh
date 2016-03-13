echo "Check if build directory exists..."
if [ -d "build" ]; 
then
	echo "Remove temporary files in build..."
	rm build/*
else
	echo "Creating build directory..."
	mkdir build
fi
echo "Copying files..."
cp src/style/* build/
cp src/slides.md build/
cp src/metadata.yaml build/
cp -r src/images build/
cd build
echo "Generating build/slides.pdf"
pandoc -t beamer -N --template=template.tex -f markdown+raw_tex --variable theme=metropolis --variable fontsize=12pt --latex-engine=xelatex slides.md metadata.yaml -o slides.pdf 
pandoc -s --mathjax -i -t revealjs -V revealjs-url=http://lab.hakim.se/reveal-js/ -V fontsize=12px slides.md metadata.yaml -o index.html
#####
# Travis check
#####
if [ -s "build/slides.pdf" ]
then 
	echo "something went wrong..."
	exit 3
else
	echo "Cleaning up..."
	rm *.sty 
	rm slides.md
	rm template.tex
	echo "Done!  (๑•̀ㅂ•́)و✧"
	exit 0
fi
