using System;

namespace MirrorList
{
    class Element
    {
        public ulong value;
        public Element left = null;
        public Element right = null;
        public Element(ulong value)
        {
            this.value = value;
        }
        public Element Direct(ulong newValue, out bool isRight)
        {
            ulong x = newValue - value;
            isRight = x > 0;
            if (x == 0) return this;
            return isRight ? this.right : this.left;
        }
    }
    class Tree
    {
        public Element root = null;
        public Element Insert(ulong newValue)
        {
            Element newEl = new Element(newValue);
            if (root is null)
            {
                root = newEl;
            }
            Element last = null;
            Element current = root;
            bool isRight = false;
            while (current != null)
            {
                last = current;
                current = last.Direct(newValue, out isRight);
                if (current != last) return null;
            }
            if (isRight) last.right = newEl;
            else last.left = newEl;
            return newEl;
        }
        public void Mirror()
        {
            Element current = root;
            Element last = null;
            Element temp = null;
            while (true)
            {
                if (current == last || current == current.left || current == current.right)
                {
                    temp = current.right;
                    current.right = last;
                    last = current.left;
                    current.left = temp;
                    if (last == current)
                    {
                        temp = current.left;
                        current.left = current.right;
                        current.right = temp;
                        break;
                    }
                    else
                    {
                        int k = 0;
                        if (last is null) k++;
                        if (current.left is null) k++;
                        if (current.right is null) k++;
                        switch (k)
                        {
                            case 0:
                                break;
                            case 1:
                                if (current.left == current.right)
                                {
                                    current.right = last;
                                    last = current.left;
                                    current.left = null;
                                }
                                else if (current.left == last)
                                {
                                    last = current.right;
                                    current.right = null;
                                }
                                else
                                {
                                    temp = current.right;
                                    current.right = last;
                                    last = current.left;
                                    current.left = temp;
                                    if ((current.left.value < current.value) && (current.value < current.right.value))
                                    {
                                        temp = current.left;
                                        current.left = current.right;
                                        current.right = temp;
                                    }
                                }
                                break;
                            case 2:
                                if (current.left is null) current.left = current.right;
                                else if (current.right is null) current.right = last;
                                else last = current;
                                    temp = current.right;
                                    current.right = last;
                                    last = current.left;
                                    current.left = temp;
                                break;
                            case 3:
                                return;
                            default:
                                break;
                        }
                        if (last != null)
                        {
                            temp = current;
                            current = last;
                            last = temp;
                        }
                    }
                }
            }
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
        }
    }
}
