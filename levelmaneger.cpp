#include "levelmaneger.h"

LevelManeger::LevelManeger() {
    levels.resize(15);
    levels[0].initLevel(3, 3, "青青子衿悠悠我心");
    levels[1].initLevel(3, 3, "锲而不舍金石可镂");
    levels[2].initLevel(3, 3, "宁为玉碎不为瓦全");
    levels[3].initLevel(3, 3, "桃李不言下自成蹊");
    levels[4].initLevel(5, 3, "此时相望不相闻愿逐月华流照君");
    levels[5].initLevel(5, 3, "人面不知何处去桃花依旧笑春风");
    levels[6].initLevel(5, 3, "江山如此多娇引无数英雄竞折腰");
    levels[7].initLevel(5, 3, "云想衣裳花想容春风拂槛露华浓");
    levels[8].initLevel(4, 4, "喜怒哀乐之未发谓之中发而皆中节");
    levels[9].initLevel(4, 4, "自诚明谓之性自明诚谓之教诚则明");
    levels[10].initLevel(5, 5, "关关雎鸠在河之洲窈窕淑女君子好逑参差荇菜左右流之");
    levels[11].initLevel(5, 5, "蒹葭苍苍白露为霜所谓伊人在水一方溯洄从之道阻且长");
    levels[12].initLevel(5, 5, "太上有立德其次有立功其次有立言虽久不废此之谓不朽");
    levels[13].initLevel(6, 6, "浔阳江头夜送客枫叶荻花秋瑟瑟主人下马客在船举酒欲饮无管弦醉不成欢惨将别");
    levels[14].initLevel(6, 6, "黄埃散漫风萧索云栈萦纡登剑阁峨嵋山下少人行旌旗无光日色薄蜀江水碧蜀山青");
}

const Level& LevelManeger::getLevel(const int& index) const
{
    return levels[index];
}
