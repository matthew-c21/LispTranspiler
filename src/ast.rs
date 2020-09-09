use crate::data::LispDatum;
use std::fmt::{Debug, Formatter};
use std::fmt;
use std::collections::HashMap;
use crate::parse::Statement;
use crate::ast::ASTNode::{Literal, Call};
use std::ops::Deref;

// NOTE(matthew-c21): Special forms to be added here.
pub enum ASTNode {
    Literal(LispDatum),
    Call(Box<ASTNode>, Vec<ASTNode>),
}

impl ASTNode {
    fn from_index(statements: Vec<Statement>, start: usize) -> Result<Vec<Self>, String> {
        let mut ast: Vec<Self> = Vec::new();

        for statement in &statements[start..] {
            match statement {
                Statement::Terminal(d) => ast.push(Literal(d.clone())),
                Statement::List(statements) => {
                    if statements.len() == 0 {
                        ast.push(Literal(LispDatum::Nil));
                    } else {
                        let stmts = statements;

                        match &statements[0] {
                            Statement::Terminal(LispDatum::Symbol(x)) => {
                                let args = ASTNode::from_index(stmts.to_vec(), 1)?;
                                ast.push(Call(Box::new(Literal(LispDatum::Symbol(x.clone()))), args))
                            }
                            _ => return Err("First element of list must be a symbol.".to_string())
                        }
                    }
                }
            }
        }

        Ok(ast)
    }

    pub fn from(statements: Vec<Statement>) -> Result<Vec<Self>, String> {
        ASTNode::from_index(statements, 0)
    }

    pub fn accept(&self, visitor: &mut Box<dyn ASTVisitor>) {
        match self {
            ASTNode::Literal(_) => visitor.visit_literal(self),
            ASTNode::Call(_, _) => visitor.visit_call(self),
        }
    }
}

// NOTE(matthew-c21): This is subject to change in response to special forms.
pub trait ASTVisitor {
    fn visit_literal(&mut self, node: &ASTNode);

    fn visit_call(&mut self, node: &ASTNode);
}

// All optimizers should be in the form ASTNode -> ASTNode.

// TODO(matthew-c21): For now, everything is run straight from the main function. Later on, I'll
//  need to break it up to account for functions and (possibly) imports.
fn preamble() -> String {
    String::from(
        "#include \"lisp.h\"\
        int main() {\
        ")
}

fn postamble() -> String {
    String::from("}")
}

pub struct TranspilationVisitor {
    content: String,
}

impl TranspilationVisitor {
    pub fn new() -> Self {
        TranspilationVisitor { content: String::new() }
    }
}

impl ASTVisitor for TranspilationVisitor {
    fn visit_literal(&mut self, node: &ASTNode) {
        let content = match node {
            Literal(x) => {
                match x {
                    LispDatum::Cons(a, b) => unimplemented!(),  // TODO(matthew-c21): I've realized that this is a terrible way to go about doing things.
                    LispDatum::Complex(r, i) => format!("new_complex({},{})", r, i),
                    LispDatum::Real(x) => format!("new_real({})", x),
                    LispDatum::Rational(p, q) => format!("new_rational({},{})", p, q),
                    LispDatum::Integer(i) => format!("new_integer({})", i),
                    LispDatum::Symbol(s) => format!("new_symbol({})", s),
                    LispDatum::Nil => format!("get_nil()"),
                }
            }
            Call(_, _) => { unreachable!() }
        };

        self.content.push_str(content.as_str());
    }

    fn visit_call(&mut self, node: &ASTNode) {
        match node {
            Literal(_) => unreachable!(),
            Call(b, args) => {
                match b.deref() {
                    Literal(LispDatum::Symbol(x)) => {
                        let mut out = String::from(x);
                        let args: Vec<String> = args.iter().map(|arg| arg.accept(Box::new(self))).collect();
                        out.push('(');
                        for arg in args {
                            out.push_str(arg.as_str());
                            out.push(',');
                        }

                        out.push(')');

                        out
                    },
                    _ => unimplemented!()
                }
            }
            _ => unimplemented!()
        }
    }
}

impl Debug for TranspilationVisitor {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(f, "{}{}{}", preamble(), self.content, postamble())
    }
}
