//
//  RTChatList.h
//  MyCppGame
//
//  Created by HanShaokun on 22/8/15.
//
//

#ifndef __MyCppGame__RTChatList__
#define __MyCppGame__RTChatList__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "RTChatNode.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::ui;

class RTChatList : public TableView, public TableViewDelegate, public TableViewDataSource
{
public:
    
    RTChatList();
    ~RTChatList();
    
    static RTChatList* create(Size size, Node *container = nullptr);
    
    void pushNode(class RTChatNode* n);
    
protected:
    
    Vector<class RTChatNode*> _sayNodeList;
    
    
#pragma mark - TableViewDelegate
    
    /**
     * Delegate to respond touch event
     *
     * @param table table contains the given cell
     * @param cell  cell that is touched
     * @js NA
     * @lua NA
     */
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    
    /**
     * Delegate to respond a table cell press event.
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     * @js NA
     * @lua NA
     */
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    
    /**
     * Delegate to respond a table cell release event
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     * @js NA
     * @lua NA
     */
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    
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
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell);
    
#pragma mark - TableViewDataSource
    
    /**
     * cell size for a given index
     *
     * @param idx the index of a cell to get a size
     * @return size of a cell at given index
     */
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    
    /**
     * cell height for a given table.
     *
     * @param table table to hold the instances of Class
     * @return cell size
     */
    virtual Size cellSizeForTable(TableView *table);
    
    /**
     * a cell instance at a given index
     *
     * @param idx index to search for a cell
     * @return cell found at idx
     */
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    
    /**
     * Returns number of cells in a given table view.
     *
     * @return number of cells
     */
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    
};

#endif /* defined(__MyCppGame__RTChatList__) */
