/*=============================================================================
    Spirit v1.6.0
    Copyright (c) 2001-2003 Daniel Nuffer
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#ifndef BOOST_SPIRIT_TREE_AST_HPP
#define BOOST_SPIRIT_TREE_AST_HPP

#include "boost/spirit/tree/common.hpp"
#include "boost/spirit/core/scanner/scanner.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

template <typename MatchPolicyT, typename NodeFactoryT>
struct ast_tree_policy;

//////////////////////////////////
//  ast_match_policy is simply an id so the correct specialization of
//  tree_policy can be found.
template <
    typename IteratorT,
    typename NodeFactoryT = node_val_data_factory<nil_t>
>
struct ast_match_policy :
    public common_tree_match_policy<
        ast_match_policy<IteratorT, NodeFactoryT>,
        IteratorT,
        NodeFactoryT,
        ast_tree_policy<
            ast_match_policy<IteratorT, NodeFactoryT>,
            NodeFactoryT
        >
    >
{
};

//////////////////////////////////
template <typename MatchPolicyT, typename NodeFactoryT>
struct ast_tree_policy :
    public common_tree_tree_policy<MatchPolicyT, NodeFactoryT>
{
    typedef
        typename common_tree_tree_policy<MatchPolicyT, NodeFactoryT>::match_t
        match_t;
    typedef typename MatchPolicyT::iterator_t iterator_t;

    static void concat(match_t& a, match_t const& b)
    {
        BOOST_SPIRIT_ASSERT(a && b);

#if defined(BOOST_SPIRIT_DEBUG) && (BOOST_SPIRIT_DEBUG_FLAGS_NODES & BOOST_SPIRIT_DEBUG_FLAGS_TREES)
        BOOST_SPIRIT_DEBUG_OUT << "concat. a = " << a <<
            "\n\tb = " << b << std::endl;
#endif
        typedef typename tree_match<iterator_t, NodeFactoryT>::container_t
            container_t;

        // test for size() is nessecary, because no_tree_gen_node leaves a.trees
        // and/or b.trees empty
        if (0 != b.trees.size() && b.trees.begin()->value.is_root())
        {
            BOOST_SPIRIT_ASSERT(b.trees.size() == 1);

            container_t tmp;
            std::swap(a.trees, tmp); // save a into tmp
            std::swap(b.trees, a.trees); // make b.trees[0] be new root (a.trees[0])
            container_t* pnon_root_trees = &a.trees;
            while (pnon_root_trees->size() > 0 &&
                    pnon_root_trees->begin()->value.is_root())
            {
                pnon_root_trees = & pnon_root_trees->begin()->children;
            }
            pnon_root_trees->insert(pnon_root_trees->begin(),
                    tmp.begin(), tmp.end());
        }
        else if (0 != a.trees.size() && a.trees.begin()->value.is_root())
        {
            BOOST_SPIRIT_ASSERT(a.trees.size() == 1);
            std::copy(b.trees.begin(),
                 b.trees.end(),
                 std::back_insert_iterator<container_t>(
                     a.trees.begin()->children));
        }
        else
        {
            std::copy(b.trees.begin(),
                 b.trees.end(),
                 std::back_insert_iterator<container_t>(a.trees));
        }

#if defined(BOOST_SPIRIT_DEBUG) && (BOOST_SPIRIT_DEBUG_FLAGS_NODES & BOOST_SPIRIT_DEBUG_FLAGS_TREES)
        BOOST_SPIRIT_DEBUG_OUT << "after concat. a = " << a << std::endl;
#endif

        return;
    }

    template <typename Iterator1T, typename Iterator2T>
    static void group_match(match_t& m, parser_id const& id,
            Iterator1T const& first, Iterator2T const& last)
    {
        if (!m)
            return;

        typedef typename tree_match<iterator_t, NodeFactoryT>::container_t
            container_t;
        typedef typename container_t::iterator cont_iterator_t;
        typedef typename NodeFactoryT::template factory<iterator_t> factory_t;
        // only one node, so don't make a new level
        if (m.trees.size() == 1)
        {
            // set rule_id's.  There may have been multiple nodes created.
            // Because of root_node[] they may be left-most children of the top
            // node.
            container_t* punset_id = &m.trees;
            while (punset_id->size() > 0 &&
                    punset_id->begin()->value.id() == 0)
            {
                punset_id->begin()->value.id(id);
                punset_id = &punset_id->begin()->children;
            }

            m.trees.begin()->value.is_root(false);
        }
        else
        {
            match_t newmatch(m.length(),
                factory_t::create_node(first, last, false));

            std::swap(newmatch.trees.begin()->children, m.trees);
            // set this node and all it's unset children's rule_id
            newmatch.trees.begin()->value.id(id);
            for (cont_iterator_t i = newmatch.trees.begin();
                 i != newmatch.trees.end();
                 ++i)
            {
                if (i->value.id() == 0)
                    i->value.id(id);
            }
            m = newmatch;
        }
    }

    template <typename FunctorT>
    static void apply_op_to_match(FunctorT const& op, match_t& m)
    {
        op(m);
    }
};

namespace impl {

    template <typename IteratorT, typename NodeFactoryT>
    struct tree_policy_selector<ast_match_policy<IteratorT, NodeFactoryT> >
    {
        typedef ast_tree_policy<
            ast_match_policy<IteratorT, NodeFactoryT>, NodeFactoryT> type;
    };

} // namespace impl


//////////////////////////////////
struct gen_ast_node_parser_gen;

template <typename T>
struct gen_ast_node_parser
:   public unary<T, parser<gen_ast_node_parser<T> > >
{
    typedef gen_ast_node_parser<T> self_t;
    typedef gen_ast_node_parser_gen parser_generator_t;
    typedef unary_parser_category parser_category_t;
//    typedef gen_ast_node_parser<T> const &embed_t;

    gen_ast_node_parser(T const& a)
    : unary<T, parser<gen_ast_node_parser<T> > >(a) {}

    template <typename ScannerT>
    typename parser_result<self_t, ScannerT>::type
    parse(ScannerT const& scan) const
    {
        typedef typename ScannerT::iteration_policy_t iteration_policy_t;
        typedef typename ScannerT::match_policy_t::iterator_t iterator_t;
        typedef typename ScannerT::match_policy_t::factory_t factory_t;
        typedef ast_match_policy<iterator_t, factory_t> match_policy_t;
        typedef typename ScannerT::action_policy_t action_policy_t;
        typedef scanner_policies<
            iteration_policy_t,
            match_policy_t,
            action_policy_t
        > policies_t;

        return this->subject().parse(scan.change_policies(policies_t()));
    }
};

//////////////////////////////////
struct gen_ast_node_parser_gen
{
    template <typename T>
    struct result {

        typedef gen_ast_node_parser<T> type;
    };

    template <typename T>
    static gen_ast_node_parser<T>
    generate(parser<T> const& s)
    {
        return gen_ast_node_parser<T>(s.derived());
    }

    template <typename T>
    gen_ast_node_parser<T>
    operator[](parser<T> const& s) const
    {
        return gen_ast_node_parser<T>(s.derived());
    }
};

//////////////////////////////////
const gen_ast_node_parser_gen gen_ast_node_d = gen_ast_node_parser_gen();


//////////////////////////////////
struct root_node_op
{
    template <typename MatchT>
    void operator()(MatchT& m) const
    {
        BOOST_SPIRIT_ASSERT(m.trees.size() > 0);
        m.trees.begin()->value.is_root(true);
    }
};

const node_parser_gen<root_node_op> root_node_d =
    node_parser_gen<root_node_op>();

///////////////////////////////////////////////////////////////////////////////
//
//  Parse functions for ASTs
//
///////////////////////////////////////////////////////////////////////////////
template <typename IteratorT, typename ParserT, typename SkipT>
tree_parse_info<IteratorT>
ast_parse(
    IteratorT const&        first_,
    IteratorT const&        last_,
    parser<ParserT> const&  parser,
    SkipT const&            skip_)
{
    typedef skip_parser_iteration_policy<SkipT> iter_policy_t;
    typedef
        scanner_policies<iter_policy_t, ast_match_policy<IteratorT> >
        scanner_policies_t;
    typedef scanner<IteratorT, scanner_policies_t> scanner_t;

    iter_policy_t iter_policy(skip_);
    scanner_policies_t policies(iter_policy);
    IteratorT first = first_;
    scanner_t scan(first, last_, policies);
    tree_match<IteratorT> hit = parser.derived().parse(scan);
    return tree_parse_info<IteratorT>(
        first, hit, hit && (first == last_), hit.length(), hit.trees);
}

//////////////////////////////////
template <typename IteratorT, typename ParserT>
tree_parse_info<IteratorT>
ast_parse(
    IteratorT const&        first_,
    IteratorT const&        last,
    parser<ParserT> const&  parser)
{
    IteratorT first = first_;
    scanner<
        IteratorT,
        scanner_policies<iteration_policy, ast_match_policy<IteratorT> >
    > scan(first, last);
    tree_match<IteratorT> hit = parser.derived().parse(scan);
    return tree_parse_info<IteratorT>(first, hit, hit && (first == last),
        hit.length(), hit.trees);
}

//////////////////////////////////
template <typename CharT, typename ParserT, typename SkipT>
tree_parse_info<CharT const*>
ast_parse(
    CharT const*            str,
    parser<ParserT> const&  parser,
    SkipT const&            skip)
{
    CharT const* last = str;
    while (*last)
        last++;
    return ast_parse(str, last, parser, skip);
}

//////////////////////////////////
template <typename CharT, typename ParserT>
tree_parse_info<CharT const*>
ast_parse(
    CharT const*            str,
    parser<ParserT> const&  parser)
{
    CharT const* last = str;
    while (*last)
    {
        last++;
    }
    return ast_parse(str, last, parser);
}

///////////////////////////////////////////////////////////////////////////////
}} // namespace boost::spirit

#endif

