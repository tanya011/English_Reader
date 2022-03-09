#include <QGroupBox>
#include "include/readnow.h"
#include <QMainWindow>
#include <QToolBar>
#include <QIcon>
#include <QAction>

ReadNow::ReadNow(QMainWindow *parent) : QMainWindow(parent) {
    auto *layout = new QHBoxLayout;

    screen_width = QApplication::desktop()->screenGeometry().width();
    screen_height = QApplication::desktop()->screenGeometry().height();

    printBook();  // выводим книгу на экран
    textEdit->setLayout(layout);
    printWindowWithTranslate();
    text->setLayout(layout);
    buttonPhraseToDict();
    createActions();     // создаем Actions
    createToolBars();    // создаем панель
};

void ReadNow::translateText() {
    if (text) {
        text->clear();
        QString trr = textEdit->textCursor().selectedText();
        QString answer =
            QString::fromStdString(translate::translate(trr.toStdString()));
        text->append(answer);
    }
}

void ReadNow::buttonPhraseToDict() {
    auto* button =  new QPushButton;
    button->setParent(this);
    button->setGeometry(screen_width - 850, 660, 600, 50);
    button->setText("Добавить в словарь");
    button->show();
}

void ReadNow::printWindowWithTranslate() {
    text = new QTextEdit(this);
    text->setGeometry(screen_width - 900, 120, 700, 500);
}

void ReadNow::printBook(const QString &book) {
    textEdit = new QTextEdit(this);
    textEdit->append(
        "Down below there was only a vast white sea of clouds. Above there was the sun, and the sun was white like the clouds, because it is never yellow when one looks at it from high in the air.\n"
        "He was still flying the Spitfire.* His right hand was on the controls. It was quite easy. The machine was flying well. He knew what he was doing.\n"
        "Everything is fine, he thought. I know my way home. I’ll be there in half an hour. When I land I shall switch off my engine and say, ‘Help me to get out, will you?’ I shall make my voice sound ordinary and natural and none of them will take any notice. Then I shall say, ‘Someone help me to get out. I can’t do it alone because I’ve lost one of my legs.’ They’ll all laugh and think I’m joking and I shall say, ‘All right, come and have a look.’ Then Yorky will climb up on to the wing and look inside. He’ll probably be sick because of all the blood and the mess. I shall laugh and say, ‘For God’s sake, help me get out.’\n"
        "He glanced down again at his right leg. There was not much of it left. The bullets had hit him, just above the knee, and now there was nothing but a great mess and a lot of blood. But there was no pain. When he looked down, he felt as if he were seeing something that did not belong to him. It was just a mess which was there; something strange and unusual and rather interesting. It was like finding a dead cat on the sofa.\n"
        "He still felt fine, and because he still felt fine, he felt excited and unafraid.\n"
        "I won’t even bother to radio for the ambulance, he thought. It isn’t necessary. And when I land I’ll sit there quite normally and say, ‘Some of you fellows come and help me out, will you, because I’ve lost one of my legs.’ I’ll laugh a little while I’m saying it; I’ll say it calmly and slowly, and they’ll think I’m joking. Then when I get out I’ll make my report. Later I’ll go up to London. I’ll take that bottle of whisky with me and I’ll give it to Bluey. We’ll sit in her room and drink it. When it’s time to go to bed, I’ll say, ‘Bluey, I’ve got a surprise for you. I lost a leg today. But I don’t mind if you don’t. It doesn’t even hurt …’ We’ll go everywhere in cars. I always hated walking.\n"
        "Then he saw the sun shining on the engine cover of his plane. He saw the sun shining on the metal, and he remembered the aeroplane and remembered where he was. He realized that he was no longer feeling good; that he was sick and his head was spinning. His head kept falling forward on to his chest because his neck no longer seemed to have any strength. But he knew that he was flying the Spitfire. Between the fingers of his right hand he could feel the handle of the stick which guided it.\n"
        "I’m going to faint, he thought. He looked at the controls. Seven thousand metres. To test himself he tried to read the hundreds as well as the thousands. Seven thousand and what? As he looked, he had difficulty reading the dial and he could not even see the needle. He knew then that he must get out; that there was not a second to lose, otherwise he would become unconscious. Quickly he tried to slide back the top, but he didn’t have the strength. For a second he took his right hand off the stick and with both hands managed to push the top back. The cold air on his face seemed to help. He had a moment of great clearness. His actions became automatic. That is what happens with a good pilot. He took some deep breaths from his oxygen mask, and as he did so, he looked out over the side. Down below there was only a vast white sea of cloud and he realized that he did not know where he was."
        "It’ll be the English Channel, he thought. I’m sure to fall in the water.\n"
        "He slowed down, pulled off his mask, undid his safety equipment and pushed the stick hard over to the left. The plane turned smoothly over on to its back and the pilot fell out.\n"
        "As he fell, he opened his eyes, because he knew that he must not become unconscious before he had opened his parachute. On one side he saw the sun; on the other he saw the whiteness of the clouds, and as he fell, as he turned in the air, the white clouds chased the sun and the sun chased the clouds. Suddenly there was no longer any sun but only a great whiteness. It was so white that sometimes it looked black, and after a while it was either white or black, but mostly it was white. He watched it as it turned from white to black, and then back to white again, and the white stayed for a long time but the black lasted only a few seconds. He seemed to go to sleep during the white periods and to wake up just in time to see the world when it was black.\n"
        "It was white when he put out a hand and touched something. He took it between his fingers and felt it. For a time he lay there, letting the tips of his fingers play with the thing which they had touched. Then slowly he opened his eyes, looked down at his hand and saw that he was holding something which was white. It was the edge of a sheet. He closed his eyes and opened them again quickly. This time he saw the room. He saw the bed in which he was lying: he saw the grey walls and the door and the green curtains over the window. There were some roses on the table by his bed and beside the roses was a small medicine glass.\n"
        "This is a hospital, he thought. I am in a hospital. But he could remember nothing. He lay back on his pillow, looking at the ceiling and wondering what had happened. He was staring at the smooth greyness of the ceiling which was so clean and grey, and then suddenly he saw a fly walking upon it. The sight of this fly touched the surface of his brain, and quickly, in that second, he remembered everything. He remembered the plane and he remembered the dial showing seven thousand metres. He remembered jumping out. He remembered his leg.\n"
        "It seemed all right now. He looked down at the end of the bed, but he could not tell. He put one hand underneath the bedclothes and felt for his knees. He found one of them, but when he felt for the other his hand touched something which was soft and covered in bandages.\n"
        "Just then the door opened and a nurse came in.\n"
        "‘Hello,’ she said. ‘So you’ve woken up at last.’\n"
        "She was not good-looking, but she was large and clean. She was between thirty and forty and she had fair hair. He did not notice more than that.\n"
        "‘Where am I?’");
    textEdit->setReadOnly(true);
    textEdit->setGeometry(40, 120, screen_width - 1000, screen_height - 300);
}

void ReadNow::createActions() {
    translateSelectedText = new QAction(tr("Translate"), this);
    translateSelectedText->setShortcut(tr("Ctrl+D"));
    translateSelectedText->setStatusTip(
        tr("Фраза будет переведена при нажатии"));
    connect(translateSelectedText, &QAction::triggered, this,
            &ReadNow::translateText);

    translateSelectedText->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)), translateSelectedText,
            SLOT(setEnabled(bool)));
}

void ReadNow::createToolBars() {
    fileToolBar = addToolBar("main toolbar");
    fileToolBar->addAction(translateSelectedText);
}

