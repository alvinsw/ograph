#include <QtGui/QApplication>
#include "mainwindow.h"
#include <vector>
#include <string>
#include <map>
#include <stdint.h>

int main(int argc, char** argv)
{
//     static const int count = 10;
//     typedef DiGraphAdjMatrix<int, int> graph;
//     graph g(count, -1);
//     graph::VertexPtr vps[count];
//     for (int i=0; i < count; ++i) {
//       vps[i] = g.AddVertex(i);
//     }
// 
//     for (int i=0; i<count; ++i) {
//       for (int j=0; j<count; ++j) {
//         if (i!=j) graph::EdgePtr e = g.AddEdge(*vps[i], *vps[j], 1);
//       }
//     }

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
    //return 0;
}
