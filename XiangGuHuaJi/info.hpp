/* XiangGuHuaJi 2016, info.hpp
 *
 */

#ifndef _XIANGGUHUAJI_INFO_H__
#define _XIANGGUHUAJI_INFO_H__

#include<iostream>
#include<vector>

#include "definition.h"

using namespace std;

//��ͼ��ĳһ����ľ������
struct PointMilitary
{    
    bool Visible;  //�˵��Ƿ�ɼ�
    bool Union;    //�˵��Ƿ�Ϊ����(���Ƿ������������Ϸű�)
    TId Owner;     //��Ȩ����
 
    vector<TMilitary> Military; //�˵صķ���������״(�˵ص�ͬ������Ƿֱ�����������˶��ٱ�)

    TDefense DefensePoints;        //�˵�Ĺ�����״
    vector<TAttack> AttackSummary; //�˵�Ĺ�����״
};


//��һ�ȡ��ȫ������
class Info
{
public:
    Info(
        TId id, TRound round, TMapSize x, TMapSize y, TId PlayerSize,
        unsigned char **  mask,
        unsigned char **  control,
        TMapPara **  MapResource,
        TMapPara **  MapDefenseRatio,
        TMapPara **  MapAttackRatio,
        TId **       Ownership,
        TMilitary *** MilitaryMap,
        TAttack *** AttackPointsMap,
        TDefense ** DefensePointsMap,
        PlayerInfo * PlayerList,
        DiplomaticStatus ** DiplomaticMap)
        : id(id), round(round), x(x), y(y), PlayerSize(PlayerSize), saving(PlayerList[id].Saving),
        mask(mask), control(control), DiplomaticMap(DiplomaticMap), Ownership(Ownership),
        MapResource(MapResource), MapDefenseRatio(MapDefenseRatio), MapAttackRatio(MapAttackRatio),
        MilitaryMap(MilitaryMap), AttackPointsMap(AttackPointsMap), DefensePointsMap(DefensePointsMap),
        PlayerList(PlayerList)
    {
        for (TId i=0; i<PlayerSize; ++i) DiplomaticCommandList[i] = KeepNeutral;
    }
    
    const TId       id;    //�Լ���ID
    const TRound    round; //��ǰ�غ���
    const TSaving   saving;//��ǰ���
    const TMapSize  x, y;  //��ͼ�ĳߴ�
    const TId       PlayerSize; //�������
    
    const unsigned char*const*const     mask;    //��ǰ�ɼ�����
    const unsigned char*const*const     control; //����Էű��ĵ���

    const DiplomaticStatus*const*const  DiplomaticMap; //ȫ���⽻״̬
    const TId*const*const Ownership;            //��ͼ��Ȩ
    const TMapPara*const*const MapResource;     //��ͼ��Դ����
    const TMapPara*const*const MapAttackRatio;  //��ͼ��������
    const TMapPara*const*const MapDefenseRatio; //��ͼ��������

    //��ȡһ�������Ϣ
    PointMilitary getPointMilitary(int i, int j) 
    {
        PointMilitary point;
        
        if (mask[i][j]) point.Visible = true;
        else { point.Visible = false; return point; }

        point.Owner = Ownership[i][j];
        point.Union = DiplomaticMap[id][point.Owner] == Union;
        
        for (TId t=0; t<PlayerSize; ++t)
        {
            point.Military.push_back(MilitaryMap[t][i][j]);
            point.AttackSummary.push_back(AttackPointsMap[t][i][j]);
        }
        point.DefensePoints = DefensePointsMap[i][j];

        return point;
    }

    //��ȡһ����ҵ���Ϣ
    PlayerInfo getPlayerInfo(TId targetId)
    {
        PlayerInfo player;
        player.id = -1;
        if (targetId<0 || targetId>=PlayerSize) return player;
        
        player = PlayerList[targetId];
        switch (DiplomaticMap[id][targetId])
        {
        case Undiscovered:
        case War:
            player.Visible = player.Union  = false;
            player.Saving = player.MapArea = player.MilitarySummary = 0;
            return player;
        case Neutral:
            player.Visible = true;
            player.Union = false;
            player.Saving = 0; 
            return player;
        case Union:
            player.Visible = player.Union  = false;
            return player;
        default:
            return player;
        }

    }

//�ɹ�д����Ϣ
    vector<DiplomaticCommand> DiplomaticCommandList;
    vector<vector<unsigned char> > MilitaryCommand;

private:
    const TMilitary*const*const*const MilitaryMap;
    const TAttack*const*const*const   AttackPointsMap;
    const TDefense*const*const        DefensePointsMap;
    const PlayerInfo*const            PlayerList;
};


#endif