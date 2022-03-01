#ifndef READNOW_H
#define READNOW_H


class ReadNow : public QWidget {
public:
    explicit ReadNow(QWidget *parent = nullptr) : QWidget(parent){
        auto horizontalGroupBox = new QGroupBox("Читаю Сейчас", this);
        QHBoxLayout *layout = new QHBoxLayout;
        std::vector<QPushButton *> buttons(5);
        for (int i = 0; i < 5; ++i) {
            buttons[i] = new QPushButton(tr("Button %1").arg(i + 1));
            layout->addWidget(buttons[i]);
        }
        horizontalGroupBox->setLayout(layout);
    }
};

#endif // READNOW_H
