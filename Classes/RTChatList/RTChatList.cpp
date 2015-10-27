//
//  RTChatList.cpp
//  MyCppGame
//
//  Created by HanShaokun on 22/8/15.
//
//

#include "RTChatList.h"
#include "DbgHelper.h"
#include "RTChatNode.h"

#define Default_Cell_Offset_Height  5
#define Default_Cell_Offset_Width   0

RTChatList::RTChatList()
{
    
}

RTChatList::~RTChatList()
{
    _sayNodeList.clear();
}

RTChatList* RTChatList::create(cocos2d::Size size, cocos2d::Node *container)
{
    
    RTChatList *pRet = new (std::nothrow) RTChatList();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
        pRet->setDataSource(pRet);
        pRet->_updateCellPositions();
        pRet->_updateContentSize();
        pRet->setVerticalFillOrder(VerticalFillOrder::TOP_DOWN);
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

int RTChatList::_maxCount = 50;
void RTChatList::setMaxCount(int count)
{
    RTChatList::_maxCount = count;
}

void RTChatList::pushNode(class RTChatNode *n, bool autoRefresh)
{
    if (!autoRefresh) {
        _sayNodeList.pushBack(n);
        if (_sayNodeList.size() > _maxCount) {
            _sayNodeList.erase(0);
        }
        return;
    }
    
    Rect rb = this->getContainer()->getBoundingBox();
    
    _sayNodeList.pushBack(n);
    if (_sayNodeList.size() > _maxCount) {
        _sayNodeList.erase(0);
    }
    this->reloadData();
    
    Rect ra = this->getContainer()->getBoundingBox();
    
    if (ra.size.height < this->getBoundingBox().size.height) {
        return;
    }
    
    if (ra.size.height > this->getBoundingBox().size.height
        && rb.size.height < this->getBoundingBox().size.height) {
        this->setContentOffset(Point::ZERO);
        return;
    }
    
    if (rb.origin.y == 0) {
        this->setContentOffset(Point::ZERO);
    }
    else {
        this->setContentOffset(Point(0, rb.origin.y - (ra.size.height - rb.size.height)));
    }
}

void RTChatList::clearNodes()
{
    _sayNodeList.clear();
    this->reloadData();
}

void RTChatList::scrollToBottom()
{
    Rect ra = this->getContainer()->getBoundingBox();
    if (ra.size.height >= this->getBoundingBox().size.height) {
        this->setContentOffset(Point::ZERO);
    }
    else {
        this->setContentOffset(Point(0, this->getBoundingBox().size.height - ra.size.height));
    }
}

#pragma mark - TableViewDelegate

/**
 * Delegate to respond touch event
 *
 * @param table table contains the given cell
 * @param cell  cell that is touched
 * @js NA
 * @lua NA
 */
void RTChatList::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

/**
 * Delegate to respond a table cell press event.
 *
 * @param table table contains the given cell
 * @param cell  cell that is pressed
 * @js NA
 * @lua NA
 */
void RTChatList::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
}

/**
 * Delegate to respond a table cell release event
 *
 * @param table table contains the given cell
 * @param cell  cell that is pressed
 * @js NA
 * @lua NA
 */
void RTChatList::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
}

/**
 * Delegate called when the cell is about to be recycled. Immediately
 * after this call the cell will be removed from the scene graph and
 * recycled.
 *
 * @param table table contains the given cell
 * @param cell  cell that is pressed
 * @js NA
 * @lua NA
 */
void RTChatList::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{
    
}

#pragma mark - TableViewDataSource

/**
 * cell size for a given index
 *
 * @param idx the index of a cell to get a size
 * @return size of a cell at given index
 */
Size RTChatList::tableCellSizeForIndex(TableView *table, ssize_t idx) {
    RTChatNode* sn = _sayNodeList.at(idx);
    if (sn->getFormType() == RTChatNode::FormType::CLEAR) {
        return Size(0, sn->getContentSize().height);
    }
    else {
        return Size(0, Default_Cell_Offset_Height + sn->getContentSize().height + Default_Cell_Offset_Height);
    }
}

/**
 * cell height for a given table.
 *
 * @param table table to hold the instances of Class
 * @return cell size
 */
Size RTChatList::cellSizeForTable(TableView *table) {
    return Size::ZERO;
}

/**
 * a cell instance at a given index
 *
 * @param idx index to search for a cell
 * @return cell found at idx
 */
TableViewCell* RTChatList::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    
    if (!cell) {
        cell = TableViewCell::create();
    }
    
    if (cell) {
        cell->removeAllChildren();
        
        auto n = _sayNodeList.at(idx);
        n->removeFromParent();
        
        if (n->getFormType() == RTChatNode::FormType::CLEAR) {
            cell->setContentSize(Size(table->getContentSize().width, n->getContentSize().height));
        }
        else {
            cell->setContentSize(Size(table->getContentSize().width, Default_Cell_Offset_Height + n->getContentSize().height + Default_Cell_Offset_Height));
        }
        
#if COCOS2D_DEBUG
        if (idx % 2 == 0) {
            DbgHelper::colorRect(cell,
                                 Point(cell->getContentSize().width / 2.0, cell->getContentSize().height / 2.0), cell->getContentSize(),
                                 Color4F::RED);
        }
        else {
            DbgHelper::colorRect(cell,
                                 Point(cell->getContentSize().width / 2.0, cell->getContentSize().height / 2.0), cell->getContentSize(),
                                 Color4F::YELLOW);
        }
#endif
        
        if (n->getFormType() == RTChatNode::FormType::BORDERED) {
            if (n->getOwnType() == RTChatNode::OwnType::SELF) {
                n->setPosition(Point(cell->getContentSize().width / 2.0 + Default_Cell_Offset_Width, cell->getContentSize().height / 2.0));
            }
            else {
                n->setPosition(Point(cell->getContentSize().width / 2.0 - Default_Cell_Offset_Width, cell->getContentSize().height / 2.0));
            }
        }
        else {
            n->setPosition(Point(cell->getContentSize().width / 2.0, cell->getContentSize().height / 2.0));
        }
        
        cell->addChild(n);
    }
    
    return cell;
}

/**
 * Returns number of cells in a given table view.
 *
 * @return number of cells
 */
ssize_t RTChatList::numberOfCellsInTableView(TableView *table)
{
    return _sayNodeList.size();
}