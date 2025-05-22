#include "levelmaneger.h"

LevelManeger::LevelManeger() {
    size = 20;
    levels.resize(size);
    levels[0].initLevel(3, 3, "清风徐来水波不兴");
    levels[1].initLevel(3, 3, "锲而不舍金石可镂");
    levels[2].initLevel(3, 3, "桃李不言下自成蹊");
    levels[3].initLevel(3, 3, "明明如月何时可掇");
    levels[4].initLevel(5, 3, "此时相望不相闻愿逐月华流照君");
    levels[5].initLevel(5, 3, "何须浅碧深红色自是花中第一流");
    levels[6].initLevel(5, 3, "江山如此多娇引无数英雄竞折腰");
    levels[7].initLevel(5, 3, "云想衣裳花想容春风拂槛露华浓");
    levels[8].initLevel(5, 3, "纤云弄巧飞星传恨银汉迢迢暗渡");
    levels[9].initLevel(4, 4, "吾充吾爱汝之心助天下人爱其所爱");
    levels[10].initLevel(4, 4, "君不见沙场征战苦至今犹忆李将军");
    levels[11].initLevel(4, 4, "剑阁峥嵘而崔嵬一夫当关万夫莫开");
    levels[12].initLevel(5, 4, "今子有五石之瓠何不虑以为大樽而浮乎江湖");
    levels[13].initLevel(5, 4, "使秦复爱六国之人则递三世可至万世而为君");
    levels[14].initLevel(5, 5, "浮长川而忘返思绵绵而增慕夜耿耿而不寐沾繁霜而至曙");//洛神赋
    levels[15].initLevel(5, 5, "蒹葭苍苍白露为霜所谓伊人在水一方溯洄从之道阻且长");
    levels[16].initLevel(5, 5, "日星隐曜山岳潜形商旅不行樯倾楫摧薄暮冥冥虎啸猿啼");
    levels[17].initLevel(5, 5, "太上有立德其次有立功其次有立言虽久不废此之谓不朽");
    levels[18].initLevel(6, 6, "浔阳江头夜送客枫叶荻花秋瑟瑟主人下马客在船举酒欲饮无管弦醉不成欢惨将别");
    levels[19].initLevel(6, 6, "黄埃散漫风萧索云栈萦纡登剑阁峨嵋山下少人行旌旗无光日色薄蜀江水碧蜀山青");

    for(int i = 0; i < size; i++)
    {
        levels[i].setId(i);
    }
}

const Level& LevelManeger::getLevel(const int& index) const
{
    return levels[index];
}

int LevelManeger::getSize() const
{
    return size;
}
