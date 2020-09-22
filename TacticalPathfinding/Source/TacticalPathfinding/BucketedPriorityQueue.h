#pragma once

#include "CoreMinimal.h"
#include "GraphNode.h"
#include "UObject/NoExportTypes.h"

template <class T>
class UBucketedPriorityQueue
{
    using Bucket = TSet<T*>;
    using Queue = TMap<float, Bucket>;
    using ValueMap = TMap<T*, float>;
    using RecordMap = TMap<const UGraphNode*, T*>;
public:
    UBucketedPriorityQueue()
    {
    }

    ~UBucketedPriorityQueue<T>()
    {
    }

    /**
     * Adds an element to the queue.
     *
     * @param element The element to be added.
     * @param bucketID The id of the bucket it should be added to.
     * @return boolean value showing if the value was added.
     */
    bool AddElement(T* element, float bucketID)
    {
        if (queue.Contains(bucketID))
        {
            queue[bucketID].Add(element);
            valueMap.Add(element, bucketID);
            recordMap.Add(element->node, element);
        }
        else
        {
            queue.Add(bucketID, Bucket());
            queue[bucketID].Add(element);
            valueMap.Add(element, bucketID);
            recordMap.Add(element->node, element);
        }
        queue.KeySort([](float A, float B) {
            return A < B;
        });
        return true;
    }

    /**
     * Removes an element to the queue.
     *
     * @param element The element to be removed from the queue.
     * @return boolean value showing if the value was removed.
     */
    bool RemoveElement(T* element)
    {
        if (!Contains(element))
        {
            return false;
        }

        float bucketID = valueMap[element];
        if (queue.Contains(bucketID) && queue[bucketID].Contains(element))
        {
            queue[bucketID].Remove(element);
            valueMap.Remove(element);
            recordMap.Remove(element->node);

            if (queue[bucketID].begin() == queue[bucketID].end())
            {
                queue.Remove(bucketID);
            }
        }

        return true;
    }

    /**
     * Modifies an element in the queue changing the bucket it is in.
     *
     * @param element The element to be modified.
     * @param oldBucketID The bucket the element should be moved from.
     * @param newBucketID The bucket the element should be moved to.
     * @return boolean value showing if the value was modified.
     */
    bool ModifyElement(T* element, float oldBucketID, float newBucketID)
    {
        Bucket* bucket = queue.Find(oldBucketID);
        if (bucket == nullptr)
        {
            return false; //Element not found.
        }

        RemoveElement(element);

        return AddElement(element, newBucketID);
    }

    /**
     * Gets the smallest element from the queue.
     *
     * @param smallestElement Pointer to memory location to store reference to the smallest element in.
     * @return boolean value showing if the value was successfully returned.
     */
    bool GetSmallestElement(T** smallestElement)
    {
        if (queue.begin() == queue.end())
        {
            return false; //Queue is empty.
        }

        const Bucket& bucket = queue.begin().Value();

        if (bucket.begin() == bucket.end())
        {
            return false; //Bucket is empty.
        }

        *smallestElement = *bucket.begin();

        return true; //Succesfully got smallest value.
    }

    /**
     * Checks if the queue is empty.
     *
     * @return boolean value showing if the queue is empty.
     */
    bool IsEmpty() const
    {
        return queue.begin() == queue.end();
    }
    
    /**
     * Checks if the queue is contains a certain element.
     *
     * @param element Element to be searched for.
     * @return boolean value showing if the queue contains a certain element.
     */
    bool Contains(T* element)
    {
        return valueMap.Contains(element);
    }
    
    /**
     * Checks if the queue is contains an element for a certain node.
     *
     * @param node The node to find the element for.
     * @return boolean value showing if the queue contains an element for a certain node.
     */
    bool ContainsByNode(const UGraphNode* node)
    {
        return recordMap.Contains(node);
    }

    /**
     * Returns the element that references a given node.
     *
     * @param element Pointer to memory location to store reference to element for the given node.
     * @param node The node to find the element for.
     * @return boolean value showing if the element could be found.
     */
    bool FindByNode(T** element, const UGraphNode* node)
    {
        if (!ContainsByNode(node))
        {
            return false;
        }
        else
        {
            *element = recordMap[node];
            return true;
        }
    }
    /**
     * Clears the queue.
     *
     */
    void Clear()
    {
        valueMap.Empty();
        queue.Empty();
    }

    int Count()
    {
        return queue.Num();
    }

private:
    Queue queue;
    ValueMap valueMap;
    RecordMap recordMap;
};
