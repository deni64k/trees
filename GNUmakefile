CXX=g++-4.8

all: rbtree rbtree.png rbtree-graph.png \
     avltree avltree.png avltree-graph.png

rbtree: rbtree.cxx rbtree.hxx
	$(CXX) -std=c++11 rbtree.cxx -o $@

rbtree.dot: rbtree
	./rbtree 2>rbtree.dot

rbtree.png: rbtree.dot tree.gv
	dot rbtree.dot | gvpr -c -ftree.gv | neato -n -Tpng -o $@

rbtree-graph.png: rbtree.dot tree.gv
	dot rbtree.dot | neato -n -Tpng -o $@

avltree: avltree.cxx avltree.hxx
	$(CXX) -std=c++11 avltree.cxx -o $@

avltree.dot: avltree
	./avltree 2>avltree.dot

avltree.png: avltree.dot tree.gv
	dot avltree.dot | gvpr -c -ftree.gv | neato -n -Tpng -o $@

avltree-graph.png: avltree.dot tree.gv
	dot avltree.dot | neato -n -Tpng -o $@

main: main.cxx rbtree.hxx tree_traversing.hxx
	$(CXX) -std=c++11 main.cxx -o $@
